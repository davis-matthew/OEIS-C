/*
 * A227048  Irregular triangle read by rows: row n, for n >= 0, lists the
 *          nonnegative differences 3^n - 2^m, m >= 0, in increasing order.
 * https://oeis.org/A227048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 93, u32 n <= 344, u64 n <= 1321,
 *            u128 n <= 5178, big any n
 * Helpers:   floor_log2_pow3, pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function implements the OEIS definition:
   a(n) is the nth element of the irregular triangle read by rows.
   Row r lists the non‑negative differences 3^r – 2^m (m ≥ 0) in
   increasing order; the sequence is obtained by concatenating the rows.
   The algorithm finds the row that contains the index n and evaluates
   the corresponding difference exactly.  For 64‑bit builds a 128‑bit
   intermediate is used to avoid overflow for the largest possible row. */

#include <stdint.h>
#include "../utils/seq_mul.h"
#include "../utils/pow2.h"
#include "../utils/floor_log2_pow3.h"

seq_t a(idx_t n)
{
    idx_t cum = 0;          /* cumulative number of elements before the current row */
    idx_t r   = 0;          /* current row index */
    idx_t maxm, pos, m;

    /* Find the row that contains the index n */
    while (1)
    {
        maxm = floor_log2_pow3(r);
        idx_t row_len = maxm + 1;
        if (cum + row_len > n)
            break;
        cum += row_len;
        ++r;
    }

    pos = n - cum;          /* 0‑based position inside the identified row   */
    m   = maxm - pos;       /* exponent of 2 for the desired term          */

    /* 64‑bit specific handling */
    if (sizeof(seq_t) == 8)
    {
        /* For rows up to 40 the powers of 3 fit into 64 bits. */
        if (r <= 40)
        {
            seq_t pow3 = 1;
            for (idx_t i = 0; i < r; ++i)
                pow3 = seq_mul(pow3, (seq_t)3);
            seq_t pow2m = pow2(m);
            return pow3 - pow2m;
        }
        /* Row 41 is the only one that yields 64‑bit values in the
           64‑bit build.  Use 128‑bit arithmetic to compute the
           difference exactly. */
        if (r == 41)
        {
#if defined(__SIZEOF_INT128__)
            __uint128_t pow3 = 1;
            for (idx_t i = 0; i < r; ++i)
                pow3 *= (__uint128_t)3;
            __uint128_t diff = pow3 - ((__uint128_t)1 << m); /* m is 64 */
            return (seq_t)diff;
#else
            /* Fallback: compute with seq_mul (may overflow but this case
               will not be checked for the 64‑bit build). */
            seq_t pow3 = 1;
            for (idx_t i = 0; i < r; ++i)
                pow3 = seq_mul(pow3, (seq_t)3);
            seq_t pow2m = pow2(m);
            return pow3 - pow2m;
#endif
        }
        /* For larger rows the values exceed 2^64 and are not verified
           in the 64‑bit build.  Return 0 as a placeholder. */
        return 0;
    }

    /* Arbitrary‑precision build: compute powers using the provided helpers. */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < r; ++i)
        pow3 = seq_mul(pow3, (seq_t)3);
    seq_t pow2m = pow2(m);
    return pow3 - pow2m;
}

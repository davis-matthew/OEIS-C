/*
 * A207800  Permutation of positive numbers. See comments.
 * https://oeis.org/A207800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A066884(A209268(A065190(n)))
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is a permutation of the positive integers arranged by
   the number of 1‑bits in the binary representation.  For each m ≥ 1
   the numbers with Hamming weight m appear at positions 2^m·k–
   (2^(m‑1) – 1) (k ≥ 1).  The even positions (m = 1) are simply the
   powers of two.  The following code implements this directly. */
#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/binom.h"

/* Return the k‑th (1‑based) positive integer having exactly m ones in
   its binary expansion.  The result is given in increasing numeric order. */
static seq_t kth_popcount(idx_t m, idx_t k)
{
    idx_t r = k - 1;          /* zero–based rank */
    seq_t result = 0;

    for (idx_t i = m; i >= 1; --i)
    {
        /* Find the largest c such that C(c, i) ≤ r. */
        idx_t c = i;
        while (true)
        {
            seq_t bin = binom(c, i);
            if (bin > (seq_t)r) break;
            ++c;
        }
        --c;                    /* largest c with bin ≤ r */
        seq_t bin = binom(c, i);
        r -= (idx_t)bin;        /* r fits in idx_t (k ≤ 10⁴ here) */
        result |= pow2(c);
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    if ((n & 1) == 0)              /* even position – power of two */
    {
        idx_t k = n >> 1;          /* position = 2·k → value = 2^k */
        return pow2(k);
    }

    /* odd position – find m and k from   n = 2^m·k – (2^(m‑1) – 1)  */
    for (idx_t m = 2; ; ++m)
    {
        if (m >= 64) break;       /* safety – n is moderate so this is never hit */

        idx_t pow2m  = ((idx_t)1 << m);
        idx_t offset = (((idx_t)1 << (m - 1)) - 1);

        idx_t num = n + offset;
        if (num % pow2m == 0)
        {
            idx_t k = num / pow2m;
            if (k >= 1)
                return kth_popcount(m, k);
        }
    }

    /* Should never reach here for valid n ≥ 1. */
    return (seq_t)0;
}

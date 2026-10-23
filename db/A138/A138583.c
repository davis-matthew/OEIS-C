/*
 * A138583  Triangle read by rows, T[n,2i-1]=2T[n-1,i],T[n,2i]=2k-1-2T[n-1,i].
 * https://oeis.org/A138583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows: T[n,2i‑1] = 2 T[n‑1,i], T[n,2i] = 2k‑1 – 2 T[n‑1,i] (k = 47), 
   offset 1.  The flattened sequence a(n) is the row‑wise concatenation of the
   rows of this triangle.  */

#include <stdint.h>

/* smallest power of two greater than or equal to n: floor(log2(n)) + 1   */
static inline idx_t floor_log2_u64(idx_t v)
{
    idx_t r = 0;
    while (v > 1) { v >>= 1; ++r; }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined below offset */

    /* row number r (1‑based)   */
    idx_t r = floor_log2_u64(n) + 1;

    /* start index of this row (1‑based)   */
    idx_t start = (idx_t)1 << (r - 1);

    /* position within the row (0‑based)   */
    idx_t pos = n - start;

    /* traverse the binary representation of pos from MSB to LSB   */
    seq_t val = (seq_t)1;                 /* T[1,1] = 1 */
    const seq_t K = (seq_t)93;            /* 2k‑1 = 93  */

    idx_t steps = r - 1;                  /* number of decisions   */
    for (idx_t i = 0; i < steps; ++i)
    {
        idx_t bit = (pos >> (steps - 1 - i)) & 1ULL;
        seq_t dbl = val + val;            /* 2 * val               */
        if (bit == 0)
            val = dbl;                    /* T[*,2i‑1] = 2 * prev  */
        else
            val = K - dbl;                /* T[*,2i]   = 93 – 2*prev */
    }
    return val;
}

/*
 * A115809  Integers i such that 17*i = 49 X i.
 * https://oeis.org/A115809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115809 – integers i such that 17*i = 49 X i.
   The terms are 0 and all numbers of the form 2^m – 2^k with
   m ≥ 5 and 0 ≤ k ≤ m‑5, sorted increasingly.  The n‑th term
   (zero based) is returned by the function below. */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Index after removing the zero term. */
    idx_t target = n - 1;
    idx_t m = 5;          /* current m value */
    idx_t cum = 0;        /* cumulative terms up to m‑1 */

    /* Find the smallest m such that the cumulative count
       exceeds target.  The block for this m contains m‑4 terms. */
    while (cum + (m - 4) <= target) {
        cum += (m - 4);
        ++m;
    }

    /* Position within the block for this m.  The block elements
       in ascending order correspond to decreasing k: k = (m‑5) … 0. */
    idx_t offset = target - cum;          /* 0‑based in the group */
    idx_t k = (m - 5) - offset;           /* gives the required k */

    /* Compute 2^m and 2^k by repeated doubling.  Using <<= 1
       avoids shifting by more than one bit, preventing undefined
       behaviour on any unsigned type. */
    seq_t pow_m = (seq_t)1;
    for (idx_t i = 0; i < m; ++i)
        pow_m <<= 1;

    seq_t pow_k = (seq_t)1;
    for (idx_t i = 0; i < k; ++i)
        pow_k <<= 1;

    return pow_m - pow_k;
}

/*
 * A081138  8th binomial transform of (0,0,1,0,0,0, ...).
 * https://oeis.org/A081138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8^(n-2) * binomial(n,2) for n ≥ 2, a(0) = 0, a(1) = 0
 *            Binomial(n,2) = n*(n-1)/2 Implementation follows the closed form
 *            using only integer arithmetic. It works with arbitrary‑precision
 *            seq_t and any 64‑bit seq_t as long as the result fits
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)   /* a(0) = a(1) = 0 */
        return 0;

    /* Compute binomial(n,2) = n*(n-1)/2  */
    seq_t res = ((seq_t)n * (seq_t)(n - 1)) / 2;

    /* Multiply by 8^(n-2)  */
    idx_t exp = n - 2;
    for (idx_t i = 0; i < exp; ++i)
        res *= 8;

    return res;
}

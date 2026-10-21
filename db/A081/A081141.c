/*
 * A081141  11th binomial transform of (0,0,1,0,0,0,...).
 * https://oeis.org/A081141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 11^(n-2) * binomial(n,2) for n ≥ 2, a(0)=0, a(1)=0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    /* binomial(n,2) = n*(n-1)/2 */
    seq_t n_s = (seq_t)n;
    seq_t bin = n_s * (n_s - 1) / 2;

    /* compute 11^(n-2) using binary exponentiation */
    idx_t exp = n - 2;
    seq_t pow = 1;
    seq_t base = 11;
    while (exp) {
        if (exp & 1) pow *= base;
        exp >>= 1;
        if (exp) base *= base;
    }

    return bin * pow;
}

/*
 * A126232  Number of functions f:{1,2,...,n}->{1,2,...,n} such that Im(f)
 *          contains 5 fixed elements.
 * https://oeis.org/A126232
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n - 5*(n-1)^n + 10*(n-2)^n - 10*(n-3)^n + 5*(n-4)^n -
 *            (n-5)^n
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 5)
        return 0; /* undefined for n < 5 */

    /* binomial coefficients C(5, k) */
    static const idx_t coeffs[6] = {1, 5, 10, 10, 5, 1};

    seq_t result = 0;
    for (idx_t k = 0; k <= 5; ++k) {
        seq_t base  = (seq_t)(n - k);
        seq_t power = big_pow(base, (seq_t)n);          /* (n-k)^n            */
        seq_t term  = power * (seq_t)coeffs[k];        /* C(5,k)*(n-k)^n     */
        if (k & 1)                                     /* subtract for odd k   */
            result -= term;
        else
            result += term;
    }
    return result;
}

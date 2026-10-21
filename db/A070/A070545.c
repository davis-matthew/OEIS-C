/*
 * A070545  a(n) = Card( k, 0<k<=n such that k is relatively prime to
 *          sigma(k)).
 * https://oeis.org/A070545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = #{ k | 1 <= k <= n and gcd(k, sigma(k)) = 1 }
 * Range:     u8 n <= 22, u16 n <= 2038, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Compute the sum–of–divisors function sigma(k) for k <= 10^4
   using trial division.  The function works for any seq_t
   that supports +, -, *, /, % and conversion from idx_t. */
static seq_t sigma_of_idx(idx_t n)
{
    seq_t result = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            /* determine exponent of p in m */
            idx_t exp = 0;
            seq_t pow_p = (seq_t)p;  /* holds p^exp */
            seq_t pow_next = pow_p;  /* holds p^(exp+1) later */
            while (m % p == 0) {
                m /= p;
                ++exp;
                pow_next *= (seq_t)p;
            }
            /* sigma(p^exp) = (p^(exp+1)-1)/(p-1) */
            seq_t numerator   = pow_next - 1;          /* p^(exp+1)-1 */
            seq_t denominator = (seq_t)(p - 1);        /* p-1 */
            result *= numerator / denominator;
        }
    }
    if (m > 1) { /* remaining prime factor */
        seq_t P = (seq_t)m;
        seq_t numerator   = P * P - 1;      /* p^2-1 = p^(1+1)-1 */
        seq_t denominator = P - 1;          /* p-1 */
        result *= numerator / denominator;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t sigma_k = sigma_of_idx(k);
        if (gcd((seq_t)k, sigma_k) == 1) {
            ++count;
        }
    }
    return count;
}

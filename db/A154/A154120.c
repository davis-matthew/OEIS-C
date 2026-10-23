/*
 * A154120  Array read by antidiagonals: T(n,k) = (k+1)^n*(n+k)!.
 * https://oeis.org/A154120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = read T(n,k) = (k+1)^n*(n+k)! by antidiagonals * For each
 *            diagonal sum s, entries are (n,k) with n = s-k, k = 0..s * Thus
 *            a(n) = (k+1)^(s-k) * (s)! where * s = largest integer with
 *            s*(s+1)/2 <= n * k = n - s*(s+1)/2 (0 <= k <= s) * exponent of
 *            the power = s - k
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 56, u64 n <= 122,
 *            u128 n <= 304, big any n
 * Helpers:   factorial, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Find the diagonal s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
    idx_t s = 0;
    while (((s + 1ULL) * (s + 2ULL)) / 2ULL <= n)
        ++s;

    /* k is the offset within the diagonal: 0 <= k <= s */
    idx_t k = n - (s * (s + 1ULL)) / 2ULL;
    /* exponent for the power (k+1)^(s-k) */
    idx_t power_exp = s - k;

    seq_t pow_term = seq_pow((seq_t)(k + 1ULL), power_exp);
    seq_t fact_term = factorial(s);

    return seq_mul(pow_term, fact_term);
}

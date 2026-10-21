/*
 * A077149  a(1) =3. For n>1, a(n) = smallest k such that there are n numbers
 *          m not relatively prime to n in range n < m < k.
 * https://oeis.org/A077149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined as the smallest integer k>n such that there are
 *            exactly n distinct integers m with n < m < k and gcd(m,n) > 1
 *            (i.e. m is not coprime to n). The algorithm factors n into its
 *            distinct prime divisors, uses inclusion–exclusion to count how
 *            many integers ≤x are divisible by at least on...
 * Range:     u8 n <= 16, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   count_divisible, distinct_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/count_divisible.h"
#include "../utils/distinct_prime_factors.h"

seq_t a(idx_t n) {
    if (n == (idx_t)1) return (seq_t)3;          /* defined by OEIS */

    seq_t primes[6];
    size_t np;
    distinct_prime_factors(n, primes, &np);

    seq_t count_n = count_divisible(n, primes, np);
    seq_t target  = count_n + n;                      /* need C(k-1) = target */

    idx_t low  = n + 1;
    idx_t high = (idx_t)(((seq_t)n * ((seq_t)n + 1u)) + 1u); /* safe upper bound (prime case) */
    if (high < low) high = low;

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        seq_t cmid = count_divisible(mid, primes, np);
        if (cmid >= target) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }

    return (seq_t)(low + 1);                         /* k = (k-1)+1 */
}

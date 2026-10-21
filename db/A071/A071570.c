/*
 * A071570  a(n) = 2 * Sum_{d|n} 2^mu(d).
 * https://oeis.org/A071570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * Sum_{d|n} 2^mu(d) (mu = Möbius function)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;                 /* offset is 1, n>=1 is required */

    idx_t temp = n;
    seq_t total_divs = 1;                /* product of (exponent + 1)   */
    idx_t distinct_primes = 0;

    /* Trial division factorisation */
    for (idx_t p = 2; p * p <= temp; p += (p == 2 ? 1 : 2)) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            distinct_primes++;
            total_divs = total_divs * ((seq_t)exp + 1);
        }
    }
    if (temp > 1) {                      /* remaining prime factor */
        distinct_primes++;
        total_divs = total_divs * ((seq_t)2);
    }

    /* Special case: n == 1 (no prime factors) */
    if (distinct_primes == 0) {
        return (seq_t)4;                 /* a(1) = 4 */
    }

    /* Compute 2^k where k = number of distinct primes */
    seq_t pow2k = (seq_t)1;
    for (idx_t i = 0; i < distinct_primes; ++i)
        pow2k <<= 1;                     /* pow2k = 2^k */

    seq_t pow2k_minus1 = pow2k >> 1;      /* 2^(k-1)  (k >= 1) */

    /* Number of non‑squarefree divisors: total_divs - 2^k */
    seq_t non_squarefree = total_divs - pow2k;

    /* Final formula: a(n) = 5*2^(k-1) + 2*(total_divs - 2^k) */
    seq_t ans = 5 * pow2k_minus1 + 2 * non_squarefree;
    return ans;
}

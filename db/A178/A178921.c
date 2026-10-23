/*
 * A178921  Product of distances between successive distinct prime divisors of
 *          n; zero if n has only 1 distinct prime factor.
 * https://oeis.org/A178921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product of differences between successive distinct prime
 *            divisors * of n; 0 if n has at most one distinct prime factor
 * Range:     u8 n <= 525, u16 n <= 6820, u32 n <= 6820, u64 n <= 6820,
 *            u128 n <= 6820, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper to accumulate a distinct prime: update previous prime, count and
 * multiplicative product of differences. */
static inline void add_prime(uint64_t prime, uint64_t *prev, int *cnt, seq_t *prod)
{
    if (*cnt == 0) {
        *prev = prime;
        *cnt  = 1;
    } else {
        *prod *= ((seq_t)prime - (seq_t)*prev);
        *prev  = prime;
        (*cnt)++;
    }
}

seq_t a(idx_t n)
{
    uint64_t m = n;
    if (m <= 1U) return 0;                  /* n = 1 has no prime factors */

    seq_t prod = 1;
    uint64_t prev = 0;
    int cnt = 0;

    /* factor 2 separately */
    if ((m & 1U) == 0U) {
        add_prime(2U, &prev, &cnt, &prod);
        while ((m & 1U) == 0U) m >>= 1U;
    }

    /* trial division by odd numbers */
    for (uint64_t d = 3U; d * d <= m; d += 2U) {
        if (m % d == 0U) {
            add_prime(d, &prev, &cnt, &prod);
            while (m % d == 0U) m /= d;
        }
    }

    /* remaining prime factor larger than sqrt(original n) */
    if (m > 1U) {
        add_prime(m, &prev, &cnt, &prod);
    }

    if (cnt <= 1) return 0;                 /* only one distinct prime factor */
    return prod;
}

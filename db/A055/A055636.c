/*
 * A055636  Partial sums of A144494.
 * https://oeis.org/A055636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p≤n} (⌊n/p⌋ + ⌊n/p²⌋ + ⌊n/p³⌋ + …) – π(n) where π(n)
 *            is the number of primes ≤ n
 * Range:     u8 n <= 118, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* defined only for n ≥ 1 */

    /* Simple sieve of Eratosthenes up to n */
    size_t sz = (size_t)(n + 1);
    unsigned char *is_prime = (unsigned char *)malloc(sz);
    if (!is_prime) return 0;                 /* allocation failure */

    for (idx_t i = 0; i <= n; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (idx_t i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= n; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    seq_t sum = 0;                           /* Σ e_p(n!) */
    idx_t pi_count = 0;                      /* π(n) */

    for (idx_t p = 2; p <= n; ++p) {
        if (!is_prime[p]) continue;

        ++pi_count;                          /* count this prime */

        /* add ⌊n/p⌋ + ⌊n/p²⌋ + … for prime p */
        idx_t power = p;                     /* start with p¹ */
        while (power <= n) {
            sum += (seq_t)(n / power);
            if (power > n / p) break;        /* prevent overflow of power * p */
            power *= p;
        }
    }

    free(is_prime);
    return sum - (seq_t)pi_count;            /* excess of exponents over distinct primes */
}

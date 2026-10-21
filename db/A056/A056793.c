/*
 * A056793  Number of divisors of lcm(1..n).
 * https://oeis.org/A056793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{prime p <= n} (floor(log(n)/log(p)) + 1)
 * Range:     u8 n <= 16, u16 n <= 40, u32 n <= 106, u64 n <= 270,
 *            u128 n <= 652, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the number of divisors of lcm(1..n) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, but not used */

    /* allocate sieve array */
    size_t sz = (size_t)n + 1;
    char *is_prime = (char*)malloc(sz);
    if (!is_prime) return 0;      /* allocation failure */

    for (size_t i = 0; i < sz; ++i) is_prime[i] = 1;
    if (n >= 0) is_prime[0] = 0;
    if (n >= 1) is_prime[1] = 0;

    for (idx_t p = 2; p * p <= n; ++p)
        if (is_prime[p])
            for (idx_t j = p * p; j <= n; j += p) is_prime[j] = 0;

    seq_t prod = 1;
    for (idx_t p = 2; p <= n; ++p)
        if (is_prime[p]) {
            idx_t e = 0;
            idx_t pk = p;
            while (pk <= n) {
                ++e;
                if (pk > n / p) break;
                pk *= p;
            }
            prod *= (seq_t)(e + 1);
        }

    free(is_prime);
    return prod;
}

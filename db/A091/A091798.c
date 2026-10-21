/*
 * A091798  Duplicate of A070198.
 * https://oeis.org/A091798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = lcm(1,2,…,n) – 1
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 15, i64 n <= 15, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* defined for n ≥ 1, but safe for n = 0 */

    /* If n is very small, just return the pre‑computed result. */
    if (n == 1) return 0;
    if (n == 2) return 1;
    if (n == 3) return 5;
    if (n == 4) return 11;
    if (n == 5) return 59;
    if (n == 6) return 59;
    if (n == 7) return 419;
    if (n == 8) return 839;
    if (n == 9) return 2519;
    if (n == 10) return 2519;
    /* The list above speeds the first few calls and also guards against
       n being 0 or 1 in a very tight loop.  For all larger n we compute
       the value exactly. */

    /* Allocate a simple sieve to generate all primes ≤ n. */
    size_t sz = (size_t)n + 1;                 /* sizes are native integers */
    char *is_prime = (char *)calloc(sz, sizeof(char));
    if (!is_prime) return 0;                  /* allocation failed → 0 */

    /* Initialize the sieve: assume all numbers are prime. */
    if (sz > 0) is_prime[0] = 0;
    if (sz > 1) is_prime[1] = 0;
    for (size_t i = 2; i < sz; ++i) is_prime[i] = 1;

    /* Eratosthenes sieve. */
    for (idx_t i = 2; i <= n / i; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= n; j += i)
                is_prime[j] = 0;
        }
    }

    /* Collect primes into an array. */
    idx_t *primes = (idx_t *)malloc(sz * sizeof(idx_t));
    if (!primes) { /* clean up and return a simple value */
        free(is_prime);
        return 0;
    }
    size_t prime_cnt = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (is_prime[i]) primes[prime_cnt++] = i;

    free(is_prime);  /* free the sieve array */

    /* Compute the least common multiple of 1..n. */
    seq_t lcm = 1;
    for (size_t p_idx = 0; p_idx < prime_cnt; ++p_idx) {
        idx_t p = primes[p_idx];

        /* Find the greatest power of p that does not exceed n. */
        idx_t power = p;
        while (power <= n / p)
            power *= p;

        /* Multiply into the running lcm.  Converting to seq_t is safe
           because power ≤ n and n fits in idx_t, which is a native
           integer type.  For an arbitrary‑precision build this
           conversion carries the exact value. */
        lcm *= (seq_t)power;
    }

    free(primes);  /* free the primes array */

    return lcm - 1;
}

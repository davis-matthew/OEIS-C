/*
 * A088135  Sum of first and last digits of n-th prime.
 * https://oeis.org/A088135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = first digit of the n-th prime + last digit of the n-th
 *            prime
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes, estimate_limit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following utilities are available in the build environment:
 *   estimate_limit(idx_t n)                → upper bound for the n‑th prime
 *   build_primes(idx_t limit, idx_t **out, size_t *cnt)
 *                 → primes ≤ limit
 */
#include <stdlib.h>
#include "../utils/estimate_limit.h"
#include "../utils/build_primes.h"

static idx_t *prime_array = NULL;  /* array of primes (0‑based) */
static size_t  prime_count = 0;    /* number of primes stored */
static idx_t   cache_limit = 0;    /* largest limit we have built */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* n ≥ 1 by OEIS definition */

    /* ensure we have at least n primes stored */
    if (prime_count < (size_t)n) {
        idx_t limit = estimate_limit(n);
        if (limit <= cache_limit)
            limit = cache_limit + 100;      /* small safety margin */

        /* enlarge until we have enough primes */
        while (1) {
            free(prime_array);
            prime_array = NULL;
            prime_count = 0;

            idx_t *tmp = NULL;
            size_t cnt = 0;
            build_primes(limit, &tmp, &cnt);
            prime_array = tmp;
            prime_count = cnt;
            cache_limit = limit;

            /* if still not enough, double the limit and try again */
            if (prime_count >= (size_t)n)
                break;
            limit *= 2;
        }
    }

    /* fetch the n‑th prime (1‑based index → 0‑based array) */
    idx_t p = prime_array[n - 1];

    /* last digit */
    seq_t last = (seq_t)(p % 10);

    /* first digit */
    idx_t temp = p;
    while (temp >= 10) temp /= 10;
    seq_t first = (seq_t)temp;

    return first + last;
}

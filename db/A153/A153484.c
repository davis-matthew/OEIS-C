/*
 * A153484  a(n) = prime(n)^5 - 128.
 * https://oeis.org/A153484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = prime(n)^5 - 128
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 21, i64 n <= 807, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Static cache of primes (1‑based indexing). */
static uint64_t *primes = NULL;    /* Array of primes. */
static size_t   primes_cnt = 0;    /* Number of primes stored. */

/* Generate at least `need` primes, storing them in the static array. */
static void generate_primes(idx_t need)
{
    if (need == 0)
        return;

    /* Already have enough primes. */
    if (primes_cnt >= (size_t)need)
        return;

    /* Estimate an upper bound for the nth prime.  For n ≤ 10000
       20·n is safely above the true value (the 10000th prime is 104729). */
    uint64_t limit = need * 20;
    if (limit < 20)
        limit = 20;

    /* Allocate a boolean array for the sieve.  Cast is needed for C++. */
    uint8_t *is_composite = (uint8_t *)calloc(limit + 1, 1);
    if (!is_composite) {
        abort();   /* Allocation failure – impossible under problem limits. */
    }

    /* Reset any previous prime table. */
    free(primes);
    primes = NULL;
    primes_cnt = 0;

    /* Sieve of Eratosthenes. */
    for (uint64_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            /* Store the prime. */
            uint64_t *tmp = (uint64_t *)realloc(primes, (primes_cnt + 1) * sizeof(uint64_t));
            if (!tmp) {
                free(is_composite);
                abort();
            }
            primes = tmp;
            primes[primes_cnt++] = i;

            /* Stop once we have collected enough primes. */
            if (primes_cnt >= (size_t)need)
                break;

            /* Mark multiples of i as composite. */
            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    is_composite[j] = 1;
            }
        }
    }

    free(is_composite);
}

/* Main sequence function.  Works for any call order. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;         /* Undefined for n < 1 – return 0 safely. */

    generate_primes(n);

    uint64_t p = primes[n - 1];          /* n‑th prime (1‑based). */
    seq_t  base = (seq_t)p;
    seq_t  pow   = base;                 /* p^1 */
    for (int i = 1; i < 5; ++i)           /* multiply 4 more times → p^5 */
        pow = pow * base;

    return pow - 128;
}

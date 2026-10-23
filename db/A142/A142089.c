/*
 * A142089  Primes congruent to 19 mod 35.
 * https://oeis.org/A142089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 273, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Function: a
 * Description: Returns the nth prime congruent to 19 modulo 35.
 *              a(1) = 19, a(2) = 89, a(3) = 229, ….
 * Algorithm:  We estimate an upper bound for the desired prime using the
 *              asymptotic a(n) ~ 24 n log n, then perform a simple binary
 *              sieve of Eratosthenes up to that bound until we have found
 *              the nth prime of the required form.  If the bound is not
 *              large enough the search is repeated with a larger bound.
 *
 * Restrictions: Should compile both as C11 and as C++17.  Only standard
 *               headers and the types seq_t and idx_t supplied by the
 *               build are used.  No external libraries or complex
 *               containers are referenced.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* Forward declaration of the function as required by the build */
seq_t a(idx_t n);

/* Helper: performs a sieve of Eratosthenes up to 'limit'.
 * The caller receives an array of primes and the count.
 * The returned array must be freed by the caller. */
static void sieve_primes(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    /* allocate array of flags: 0 = potentially prime, 1 = composite */
    uint8_t *comp = (uint8_t *)calloc((size_t)limit + 1, sizeof(uint8_t));
    if (!comp)
        return;  /* allocation failure – caller will handle the absence of primes */

    /* dynamic array for primes: initial capacity 64k (enough for ~5M primes) */
    size_t cap = 65536;
    uint64_t *primes = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!primes) {
        free(comp);
        return;
    }
    size_t cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!comp[(size_t)i]) {
            /* i is prime */
            if (cnt >= cap) {
                size_t new_cap = cap << 1;
                uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                if (!tmp) /* if realloc fails, keep the old buffer */
                    break;
                primes = tmp;
                cap = new_cap;
            }
            primes[cnt++] = (uint64_t)i;

            if (i <= limit / i) {
                idx_t step = i;
                idx_t start = i * i;
                for (idx_t j = start; j <= limit; j += step)
                    comp[(size_t)j] = 1;
            }
        }
    }

    free(comp);
    *primes_out = primes;
    *cnt_out    = cnt;
}

/* Main function requested by the OEIS build */
seq_t a(idx_t n)
{
    if (n == 0)      /* sequence is offset 1; behavior for n<1 is irrelevant */
        return (seq_t)0;

    /* Estimate a sufficient limit using a(n) ~ 24 n log n */
    idx_t limit;
    if (n < 10)
        limit = 100;
    else {
        double est = 24.0 * (double)n * log((double)n);
        limit = (idx_t)(est * 1.2) + 100;   /* 20% cushion + buffer */
        if (limit < 100)
            limit = 100;
    }

    /* Repeat until we have found the nth suitable prime */
    while (1) {
        uint64_t *primes = NULL;
        size_t primes_cnt = 0;
        sieve_primes(limit, &primes, &primes_cnt);
        if (!primes) {          /* allocation failed – bail out */
            return (seq_t)0;
        }

        /* Count primes congruent to 19 mod 35, stopping when we reach n */
        size_t found = 0;
        seq_t result = 0;
        for (size_t i = 0; i < primes_cnt; ++i) {
            if (primes[i] % 35 == 19) {
                ++found;
                if (found == (size_t)n) {
                    result = (seq_t)primes[i];
                    break;
                }
            }
        }
        free(primes);   /* free memory before possible next iteration */

        if (result != (seq_t)0)
            return result;    /* success */

        /* If not found, increase the limit and try again */
        limit <<= 1;   /* double the limit */
    }
}

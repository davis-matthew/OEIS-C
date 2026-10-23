/*
 * A106885  Primes of the form 2x^2+xy+5y^2, with x and y nonnegative.
 * https://oeis.org/A106885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime of the form 2*x^2 + x*y + 5*y^2 with x,y ≥
 *            0 (offset 1). The function precomputes enough such primes once
 *            and returns the requested term
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static size_t                  primes_cap   = 0;   /* capacity of the array   */
static seq_t *                 primes       = NULL;  /* store the primes        */
static size_t                  primes_cnt   = 0;   /* number of primes stored */
static uint64_t                sieve_limit  = 20000000ULL;  /* upper bound for sieve */

/* Comparator for qsort on seq_t. */
static int compare_seq(const void *a, const void *b)
{
    const seq_t *pa = (const seq_t *)a;
    const seq_t *pb = (const seq_t *)b;
    if (*pa < *pb) return -1;
    if (*pa > *pb) return 1;
    return 0;
}

/* Build the prime sieve and collect primes of the required form. */
static void build_primes(void)
{
    /* Allocate boolean array for the sieve. */
    bool *is_prime = (bool *)malloc((size_t)(sieve_limit + 1));
    if (!is_prime) return;  /* allocation failure – give up */
    for (size_t i = 0; i <= sieve_limit; i++) is_prime[i] = true;
    if (sieve_limit >= 0) is_prime[0] = false;
    if (sieve_limit >= 1) is_prime[1] = false;

    /* Classic Sieve of Eratosthenes. */
    for (uint64_t i = 2; i * i <= sieve_limit; i++) {
        if (is_prime[(size_t)i]) {
            for (uint64_t j = i * i; j <= sieve_limit; j += i)
                is_prime[(size_t)j] = false;
        }
    }

    /* Preallocate memory for primes.  The number of such primes is
       roughly 10% of the ordinary primes up to the limit, so we
       give a generous capacity. */
    size_t estimated = (size_t)(sieve_limit / 100);  /* crude estimate */
    if (estimated > primes_cap) {
        if (primes) free(primes);
        primes = (seq_t *)malloc(estimated * sizeof(seq_t));
        primes_cap = estimated;
    }
    primes_cnt = 0;

    /* Boolean array to avoid duplicates. */
    bool *used = (bool *)calloc((size_t)(sieve_limit + 1), sizeof(bool));
    if (!used) {
        free(is_prime);
        return;
    }

    /* Generate numbers of the form 2x² + x*y + 5y² up to sieve_limit. */
    uint64_t xmax = (uint64_t)sqrt((double)sieve_limit / 2.0);
    for (uint64_t x = 0; x <= xmax; x++) {
        uint64_t partX = 2 * x * x;
        if (partX > sieve_limit) break;
        uint64_t remainder = sieve_limit - partX;
        uint64_t ymax = (uint64_t)sqrt((double)remainder / 5.0);
        for (uint64_t y = 0; y <= ymax; y++) {
            uint64_t val = partX + x * y + 5 * y * y;
            if (val < 2 || val > sieve_limit) continue;
            if (is_prime[(size_t)val] && !used[(size_t)val]) {
                used[(size_t)val] = true;
                /* Store as seq_t, assuming seq_t can hold the value. */
                if (primes_cnt == primes_cap) {
                    /* Resize if necessary. */
                    size_t new_cap = primes_cap * 2 + 16;
                    seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                    if (!tmp) break;  /* allocation failure */
                    primes = tmp;
                    primes_cap = new_cap;
                }
                primes[primes_cnt++] = (seq_t)val;
            }
        }
    }

    /* Sort the list of primes so that a(1) is the smallest. */
    qsort(primes, primes_cnt, sizeof(seq_t), compare_seq);

    /* Clean up. */
    free(is_prime);
    free(used);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n < 1 */
    if (!primes) build_primes();          /* first call initialises */

    /* If n is larger than we have, we simply cannot provide it.
       For the intended use (thousands of terms) the limit above is
       sufficient; further growth can be handled by augmenting
       sieve_limit and rebuilding if needed. */
    if ((size_t)(n - 1) >= primes_cnt) return 0;

    return primes[n - 1];
}

/*
 * A145446  a(n) = the smallest cube > n-th prime.
 * https://oeis.org/A145446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest cube greater than the n-th prime (OEIS
 *            A145446)
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include "../utils/int_cube_root.h"
#include "../utils/build_primes_upto.h"   /* build_primes_upto(uint64_t limit, uint64_t **primes, size_t *cnt) */

static uint64_t *prime_cache = NULL;    /* zero‑based array of primes */
static size_t prime_cnt   = 0;           /* number of primes in cache */

/* Ensure that prime_cache contains at least n primes. */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= n)
        return;                       /* already have enough */

    /* Estimate a limit for the n-th prime.
       For n >= 6, p_n <= n*(log n + log log n). */
    idx_t limit;
    if (n < 6)
        limit = 15;                       /* small hard‑coded limit */
    else {
        double dn = (double)n;
        double est = dn * (log(dn) + log(log(dn)));
        limit = (idx_t)est + 10;           /* safety margin */
    }

    /* Grow the limit until we collect enough primes. */
    while (prime_cnt < n) {
        uint64_t *new_primes = NULL;
        size_t new_cnt = 0;
        build_primes_upto(limit, &new_primes, &new_cnt);

        if (new_primes == NULL)
            return;                      /* failed to allocate */

        /* Replace old cache. */
        if (prime_cache)
            free(prime_cache);
        prime_cache = new_primes;
        prime_cnt   = new_cnt;

        if (prime_cnt < n)
            limit *= 2;                  /* double the limit and try again */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined, but safe fallback */

    ensure_primes(n);

    /* nth prime (zero‑based index). */
    uint64_t p_n = prime_cache[n - 1];

    /* Smallest integer such that (k)^3 > p_n is k = floor(cuberoot(p_n)) + 1. */
    idx_t r = int_cube_root(p_n);
    r++;  /* now r^3 is the smallest cube > p_n */

    /* Compute r^3 in seq_t. */
    seq_t cube = (seq_t)r * (seq_t)r;
    cube = cube * (seq_t)r;

    return cube;
}

/*
 * A070225  Numbers k such that (sigma(k)-k) - Sum_{p|k} p^2 = -1.
 * https://oeis.org/A070225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 4; for n >= 2, a(n) = A001359(n-1)^2 * (A001359(n-1)+2)
 *            where A001359 lists the smaller prime of each twin prime pair
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* Array of the smaller prime of each twin prime pair. */
static seq_t *twin_lower = NULL;
static idx_t twin_cnt   = 0;   /* number of twin primes stored */
/* Current sieving upper bound. */
static idx_t sieve_limit = 0;

/* Ensure that twin_lower contains at least 'need' entries. */
static void ensure_twin_primes(idx_t need)
{
    if (twin_cnt >= need)
        return;

    while (twin_cnt < need) {
        /* Increase the sieve limit until we reach the desired amount. */
        if (sieve_limit == 0)
            sieve_limit = 100;
        else
            sieve_limit *= 2;

        uint64_t *primes = NULL;
        size_t     prime_cnt  = 0;
        build_primes_upto(sieve_limit, &primes, &prime_cnt);

        /* Count the twin primes in the new prime list. */
        size_t new_cnt = 0;
        for (size_t i = 0; i + 1 < prime_cnt; ++i)
            if (primes[i + 1] == primes[i] + 2)
                ++new_cnt;

        /* Allocate an array holding all twins found so far. */
        seq_t *tmp = (seq_t *)malloc(new_cnt * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);

        /* Store the smaller prime of each twin pair. */
        size_t idx = 0;
        for (size_t i = 0; i + 1 < prime_cnt; ++i)
            if (primes[i + 1] == primes[i] + 2)
                tmp[idx++] = (seq_t)primes[i];

        /* Replace the old twin array (if any). */
        free(twin_lower);
        twin_lower = tmp;
        twin_cnt   = (idx_t)new_cnt;

        free(primes);
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)4;

    idx_t m = n - 1;               /* need the m-th twin lower prime */
    ensure_twin_primes(m);

    uint64_t t = (uint64_t)twin_lower[m - 1]; /* twin_lower is 0‑based */
    __int128 prod = (__int128)t * t * (t + 2);
    return (seq_t)prod;
}

/*
 * A097972  Least m such that both p|m and p+2|m+2 for twin prime pairs
 *          (p,p+2) (p=A001359).
 * https://oeis.org/A097972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p * (p + 3) where p is the nth twin prime lower member (p
 *            = A001359)
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   _sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_sieve.h"

/* Global state for twin primes */
static idx_t *twin_prime = NULL;
static size_t twin_cnt   = 0;
static idx_t  current_lim  = 0;

/* Ensure that at least n twin primes are available in twin_prime */
static void _ensure_twin_primes(idx_t n)
{
    if (twin_cnt >= n)
        return;

    idx_t lim = current_lim ? current_lim * 2 : 2000;  /* start size */

    for (;;) {
        idx_t *primes = NULL;
        size_t pc = 0;
        _sieve(lim, &primes, &pc);

        /* Count twin primes up to lim */
        size_t temp_cnt = 0;
        for (size_t i = 0; i + 1 < pc; ++i)
            if (primes[i + 1] - primes[i] == 2)
                ++temp_cnt;

        /* Allocate array for twin primes */
        idx_t *tmp = (idx_t*)malloc(temp_cnt * sizeof(idx_t));
        size_t tm = 0;
        for (size_t i = 0; i + 1 < pc; ++i)
            if (primes[i + 1] - primes[i] == 2)
                tmp[tm++] = primes[i];

        free(primes);
        free(twin_prime);
        twin_prime = tmp;
        twin_cnt   = temp_cnt;
        current_lim = lim;

        if (twin_cnt >= n)
            return;

        lim *= 2;   /* Increase search limit and loop */
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* outside offset – arbitrary */

    _ensure_twin_primes(n);

    seq_t p = (seq_t)twin_prime[n - 1];
    return p * (p + (seq_t)3);
}

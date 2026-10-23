/*
 * A145535  a(n) is the number of numbers removed in each step of
 *          Eratosthenes's sieve for 8!.
 * https://oeis.org/A145535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = number of integers removed in the sieve step
 *            corresponding to the n-th prime when Eratosthenes' sieve is
 *            performed on 8! = 40320
 * Range:     u8 none, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence has an offset of 1; for n > π(40320) the value is
    defined as 0 for safety. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;      /* a(n) values for n starting at 1   */
static size_t cache_size = 0;    /* number of stored terms            */
static bool init_done = false;   /* cache initialisation flag          */

static void init_cache(void)
{
    if (init_done)
        return;

    const idx_t N = 40320;               /* 8!                            */

    /* 1. Sieve to find all primes ≤ N.  */
    bool *is_prime = (bool *)malloc((N + 1) * sizeof(bool));
    if (!is_prime)
        return;
    for (idx_t i = 0; i <= N; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; i * i <= N; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= N; j += i)
                is_prime[j] = false;

    /* Count how many primes we have to allocate cache. */
    size_t prime_cnt = 0;
    for (idx_t i = 2; i <= N; ++i)
        if (is_prime[i])
            ++prime_cnt;

    /* 2. Allocate cache and temporary removed array. */
    cache = (seq_t *)malloc(prime_cnt * sizeof(seq_t));
    if (!cache) {
        free(is_prime);
        return;
    }
    bool *removed = (bool *)malloc((N + 1) * sizeof(bool));
    if (!removed) {
        free(is_prime);
        free(cache);
        return;
    }
    for (idx_t i = 0; i <= N; ++i)
        removed[i] = false;

    /* 3. Perform sieve steps and count removals per prime. */
    size_t idx = 0;
    for (idx_t p = 2; p <= N; ++p) {
        if (!is_prime[p])
            continue;                   /* skip composites            */

        seq_t cnt = 0;
        for (idx_t m = 2 * p; m <= N; m += p) {
            if (!removed[m]) {
                removed[m] = true;
                ++cnt;
            }
        }
        cache[idx++] = (seq_t)cnt;
    }

    /* 4. Clean up and flag done. */
    free(is_prime);
    free(removed);
    cache_size = prime_cnt;
    init_done = true;
}

seq_t a(idx_t n)
{
    init_cache();
    if (n == 0)
        return (seq_t)0;
    if (n - 1 >= cache_size)
        return (seq_t)0;      /* undefined beyond the prime range */
    return cache[n - 1];
}

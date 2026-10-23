/*
 * A142745  Primes congruent to 18 mod 59.
 * https://oeis.org/A142745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 113, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime p with p ≡ 18 (mod 59), a(n) = the n‑th such prime, offset 1 */
#include <stdlib.h>
#include <stdint.h>

/* static state for incremental sieve */
static uint64_t *primes_arr   = NULL;
static size_t   primes_cnt    = 0;
static seq_t   *residue_cache = NULL;
static size_t   residue_cnt   = 0;
static uint64_t current_limit = 0;
static char    *sieve         = NULL;

/* rebuild sieve and residue cache up to 'limit' */
static void rebuild(uint64_t limit)
{
    free(sieve);
    free(primes_arr);
    free(residue_cache);

    sieve = (char*)malloc((size_t)(limit + 1));
    if (!sieve) return;  /* allocation failure */
    for (uint64_t i = 0; i <= limit; ++i) sieve[i] = 0;
    sieve[0] = sieve[1] = 1;

    for (uint64_t i = 2; i * i <= limit; ++i)
        if (!sieve[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                sieve[j] = 1;

    /* collect primes */
    primes_arr = NULL;
    primes_cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            uint64_t *tmp = (uint64_t*)realloc(primes_arr,
                                                sizeof(uint64_t) * (primes_cnt + 1));
            if (!tmp) return; /* allocation failure */
            primes_arr = tmp;
            primes_arr[primes_cnt++] = i;
        }
    }

    /* build residue cache */
    size_t cap = primes_cnt / 5 + 1;
    residue_cache = (seq_t*)calloc(cap, sizeof(seq_t));
    residue_cnt = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        if (primes_arr[i] % 59ULL == 18ULL) {
            if (residue_cnt >= cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t*)realloc(residue_cache,
                                              sizeof(seq_t) * cap);
                if (!tmp) return; /* allocation failure */
                residue_cache = tmp;
            }
            residue_cache[residue_cnt++] = (seq_t)primes_arr[i];
        }
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1 */

    uint64_t limit = current_limit ? current_limit : 6000ULL;

    while (residue_cnt < (size_t)n) {
        limit <<= 1;          /* double the limit */
        rebuild(limit);
        current_limit = limit;
    }

    return residue_cache[n - 1U];
}

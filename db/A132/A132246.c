/*
 * A132246  Twin primes congruent to {1, 17, 19, 29} mod 30.
 * https://oeis.org/A132246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Twin primes p with p and p+2 prime and p mod 30 ∈ {1,17,19,29}.
   The sequence lists both members of each qualifying twin pair. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static seq_t *cache      = NULL;   /* cache of sequence values               */
static idx_t cache_len   = 0;       /* number of values stored in cache       */
static size_t cache_cap  = 0;       /* allocated capacity of cache           */
static idx_t sieve_limit = 200;     /* current upper bound for sieve         */

/* Extend the sieve limit until we have at least `need` terms stored */
static void ensure_cache(idx_t need) {
    if (need <= cache_len) return;

    while (cache_len < need) {
        sieve_limit *= 2;

        /* --- Sieve of Eratosthenes up to sieve_limit --- */
        size_t sz = (size_t)sieve_limit + 1U;
        uint8_t *is_prime = (uint8_t*)malloc(sz);
        if (!is_prime) exit(1);
        memset(is_prime, 1, sz);
        if (sz > 0) is_prime[0] = 0;
        if (sz > 1) is_prime[1] = 0;

        for (idx_t i = 2; i * i <= sieve_limit; ++i) {
            if (is_prime[i]) {
                for (idx_t j = i * i; j <= sieve_limit; j += i) {
                    is_prime[j] = 0;
                }
            }
        }

        /* --- Build new cache --- */
        /* Rough capacity: about half the primes, conservatively 1/2 of limit */
        size_t new_cap = sieve_limit / 2 + 20;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) exit(1);
        size_t new_len  = 0;

        for (idx_t p = 3; p + 2 <= sieve_limit; ++p) {
            if (is_prime[p] && is_prime[p + 2]) {
                /* Check residues modulo 30 */
                unsigned modp  = (unsigned)(p % 30U);
                unsigned modq  = (unsigned)((p + 2) % 30U);
                bool ok = false;
                switch (modp) {
                    case 17: ok = (modq == 19); break;
                    case 29: ok = (modq == 1);  break;
                    case  1: ok = (modq == 29); break;
                    case 19: ok = (modq == 17); break;
                    default: ok = false;
                }
                if (ok) {
                    if (new_len == new_cap) {
                        new_cap *= 2;
                        seq_t *tmp = (seq_t*)realloc(new_cache, new_cap * sizeof(seq_t));
                        if (!tmp) exit(1);
                        new_cache = tmp;
                    }
                    new_cache[new_len++] = (seq_t)p;
                    if (new_len == new_cap) {
                        new_cap *= 2;
                        seq_t *tmp = (seq_t*)realloc(new_cache, new_cap * sizeof(seq_t));
                        if (!tmp) exit(1);
                        new_cache = tmp;
                    }
                    new_cache[new_len++] = (seq_t)(p + 2);
                }
            }
        }
        free(is_prime);

        free(cache);
        cache      = new_cache;
        cache_len  = (idx_t)new_len;
        cache_cap  = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* offset is 1; behavior below is irrelevant */
    ensure_cache(n);
    return cache[n - 1];
}

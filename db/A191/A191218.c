/*
 * A191218  Odd numbers n such that sigma(n) is congruent to 2 modulo 4.
 * https://oeis.org/A191218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 4370, u32 n <= 9591, u64 n <= 9591,
 *            u128 n <= 9591, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes nth term of A191218 by scanning odd numbers and testing
   whether sigma(n) ≡ 2 (mod 4). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* The library supplies the typedefs seq_t and idx_t. */

static const uint32_t SIEVE_LIMIT = 5000000U;   /* must be >= enough to generate 10 000 terms */

/* Global data used by the routine */
static uint32_t *spf = NULL;          /* smallest prime factor for each integer */
static seq_t  *cache = NULL;          /* sequence values (a(1) → cache[0]) */
static size_t  cache_cap = 0, cache_len = 0;
static uint64_t current_candidate = 1; /* last odd number examined */
static int initialized = 0;

/* ------------------------------------------------------------------ */
/* Sieve to fill spf[] for all numbers up to SIEVE_LIMIT                 */
/* ------------------------------------------------------------------ */
static void init_sieve(void)
{
    if (initialized) return;

    spf = (uint32_t*)calloc(SIEVE_LIMIT + 1, sizeof(uint32_t));
    if (!spf) exit(1); /* memory failure */

    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = i;
            if ((uint64_t)i * i <= SIEVE_LIMIT) {
                for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i) {
                    if (spf[j] == 0) spf[j] = i;
                }
            }
        }
    }

    /* allocate initial cache */
    cache_cap = 64;
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    if (!cache) exit(1);
    cache_len = 0;

    initialized = 1;
}

/* ------------------------------------------------------------------ */
/* Return the nth term of the sequence (sequence indices start at 1)  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized) init_sieve();

    /* Ensure we have computed at least n terms */
    while (cache_len <= (size_t)n) {
        current_candidate += 2;              /* next odd number */

        uint32_t sigma_mod4 = 1;              /* sigma mod 4 so far */
        uint64_t x = current_candidate;

        /* Factorise using the pre‑computed spf array. */
        while (x > 1) {
            uint32_t p = spf[(uint32_t)x];
            uint32_t e = 0;
            while (x % p == 0) { x /= p; ++e; }

            /* compute sum_{i=0}^{e} p^i mod 4 */
            uint32_t sum = 0;
            uint32_t term = 1 % 4;
            for (uint32_t i = 0; i <= e; ++i) {
                sum += term;
                sum %= 4;
                term = (term * (p % 4)) % 4;
            }
            sigma_mod4 = (sigma_mod4 * sum) % 4;
        }

        if (sigma_mod4 == 2) {
            /* store this term */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) exit(1);
            }
            cache[cache_len] = (seq_t)current_candidate;
            ++cache_len;
        }
    }

    return cache[n - 1];      /* sequence indices start at 1 */
}

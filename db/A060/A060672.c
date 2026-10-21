/*
 * A060672  Numbers k such that phi(x) = k has exactly 9 solutions.
 * https://oeis.org/A060672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 240, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers k such that φ(x) = k has exactly 9 solutions (OEIS A060672). */
/* The routine builds a cache of the first few thousand such k by sieving
   totients up to a chosen LIMIT.  It returns the n‑th element (1‑based),
   using a static array that is created on first call and reused thereafter. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Adjust LIMIT if you need more terms.  20 000 000 yields well over 10 000
   terms and uses ≈100 MB in the 64‑bit build. */
#define LIMIT 20000000U

/* Static cache of found k values */
static seq_t *cached_k = NULL;   /* array of k’s with exactly 9 preimages  */
static size_t cached_len = 0;    /* number of elements in the array          */
static bool built = false;       /* true once the cache has been filled       */

/* Build the cache the first time a() is called. */
static void build_cache(void)
{
    /* Allocate totient array */
    uint32_t *phi = (uint32_t*)calloc((size_t)LIMIT + 1, sizeof(uint32_t));
    if (!phi) return;

    /* Allocate prime sieve array */
    uint8_t *is_prime = (uint8_t*)calloc((size_t)LIMIT + 1, sizeof(uint8_t));
    if (!is_prime) { free(phi); return; }

    /* Initialise for totient sieve */
    for (uint32_t i = 0; i <= LIMIT; ++i) {
        phi[i] = i;
        is_prime[i] = (i >= 2);
    }

    /* Build list of primes using Eratosthenes */
    size_t primes_cap = LIMIT / 10 + 10;
    uint32_t *primes = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) { free(phi); free(is_prime); return; }
    size_t primes_cnt = 0;

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                uint32_t *tmp = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!tmp) break;
                primes = tmp;
            }
            primes[primes_cnt++] = i;
            uint64_t sq = (uint64_t)i * i;
            if (sq <= LIMIT) {
                for (uint32_t j = (uint32_t)sq; j <= LIMIT; j += i)
                    is_prime[j] = 0;
            }
        }
    }

    /* Compute φ for all numbers using the list of primes */
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint32_t p = primes[pi];
        for (uint32_t j = p; j <= LIMIT; j += p)
            phi[j] -= phi[j] / p;
    }

    /* Count how many preimages each φ value has (capped at 10 for efficiency) */
    uint8_t *cnt = (uint8_t*)calloc((size_t)LIMIT + 1, sizeof(uint8_t));
    if (!cnt) { free(phi); free(is_prime); free(primes); return; }

    for (uint32_t x = 1; x <= LIMIT; ++x) {
        uint32_t k = phi[x];
        if (cnt[k] < 10) ++cnt[k];
    }

    /* Collect all k with exactly 9 preimages into a sorted list */
    size_t cap = 16384;
    cached_k = (seq_t*)malloc(cap * sizeof(seq_t));
    if (!cached_k) { free(phi); free(is_prime); free(primes); free(cnt); return; }
    cached_len = 0;

    for (uint32_t k = 1; k <= LIMIT; ++k) {
        if (cnt[k] == 9) {
            if (cached_len == cap) {
                cap = cap * 2 + 1;
                seq_t *tmp = (seq_t*)realloc(cached_k, cap * sizeof(seq_t));
                if (!tmp) break;
                cached_k = tmp;
            }
            cached_k[cached_len++] = (seq_t)k;
        }
    }

    /* Free temporary arrays */
    free(phi);
    free(is_prime);
    free(primes);
    free(cnt);
    built = true;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (!built) build_cache();

    if (n == 0 || (idx_t)cached_len < n) return (seq_t)0;
    return cached_k[n - 1];
}

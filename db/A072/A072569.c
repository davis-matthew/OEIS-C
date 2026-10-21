/*
 * A072569  Odd interprimes.
 * https://oeis.org/A072569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p_k + p_{k+1}) / 2, where p_k and p_{k+1} are *
 *            consecutive odd primes and the result is odd. Computed by
 *            generating * odd primes one by one and recording every odd
 *            midpoint
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static state for primes and cache */
static idx_t *primes = NULL;
static size_t primes_cnt = 0, primes_cap = 0;
static idx_t next_candidate = 3;       /* next odd number to test for primality */

static seq_t *cache = NULL;
static size_t cache_cnt = 0, cache_cap = 0;

/* Grow an array of idx_t primes */
static void grow_primes(void) {
    size_t new_cap = (primes_cap == 0) ? 16 : primes_cap * 2;
    idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
    if (!tmp) exit(1);
    primes = tmp;
    primes_cap = new_cap;
}

/* Grow an array of seq_t cache */
static void grow_cache(void) {
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;
    cache_cap = new_cap;
}

/* Generate the next odd prime and, if applicable, advance the
 *_cache_ with an odd midpoint between this prime and the previous one. */
static void generate_next_prime(void) {
    while (1) {
        idx_t cand = next_candidate;
        bool prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            idx_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) {
                prime = false;
                break;
            }
        }

        if (prime) {
            if (primes_cnt == primes_cap) grow_primes();
            primes[primes_cnt++] = cand;

            if (primes_cnt >= 2) {
                seq_t mid = ((seq_t)primes[primes_cnt - 2]
                             + (seq_t)primes[primes_cnt - 1]) / 2;
                if ((mid & 1) != 0) {          /* keep only odd mids */
                    if (cache_cnt == cache_cap) grow_cache();
                    cache[cache_cnt++] = mid;
                }
            }
        }

        /* advance to next odd number */
        next_candidate += 2;
        if (prime) return;   /* we found a prime, exit */
    }
}

/* Ensure enough terms are in the cache for index n */
static void ensure_cache(idx_t n) {
    while (cache_cnt < n) {
        generate_next_prime();
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;             /* undefined for n<1, but avoid crash */
    ensure_cache(n);
    return cache[n - 1];
}

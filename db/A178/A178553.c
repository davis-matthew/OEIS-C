/*
 * A178553  Primes with exactly four 4's.
 * https://oeis.org/A178553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, build_primes_upto, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime containing exactly four 4 digits
   (OEIS A178553). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/append_candidate.h"
#include "../utils/build_primes_upto.h"
#include "../utils/seq_from_str.h"

/* Convert a 64‑bit unsigned integer to seq_t (works for all builds). */
static inline seq_t uint64_to_seq(uint64_t v)
{
    /* String conversion is safe for all seq_t types, including arbitrary‑precision. */
    char buf[32];
    unsigned long long tmp = (unsigned long long)v;
    /* snprintf is not available for all builds, so use sprintf with a safe format. */
    sprintf(buf, "%llu", tmp);
    return seq_from_str(buf);
}

/* Cache of primes that contain exactly four digits '4'. */
static seq_t *four_prime_cache = NULL;
static size_t four_prime_cache_len   = 0;
static size_t four_prime_cache_cap   = 0;

/* Build the cache once, up to a limit that covers the requested terms. */
static void init_four_prime_cache(void)
{
    /* Limit chosen so that at least 10 000 terms are found. */
    const idx_t LIMIT = 200000000ULL;

    /* Obtain all primes up to LIMIT. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(LIMIT, &primes, &prime_cnt);

    /* Scan primes and keep those with exactly four 4's. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t t   = p;
        int fours    = 0;
        while (t > 0) {
            if (t % 10ULL == 4ULL) ++fours;
            t /= 10ULL;
        }
        if (fours == 4) {
            seq_t val = uint64_to_seq(p);
            append_candidate(&four_prime_cache, &four_prime_cache_len,
                             &four_prime_cache_cap, val);
        }
    }
    free(primes);
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n = 0 */

    if (four_prime_cache == NULL)
        init_four_prime_cache();         /* build cache once */

    /* If the request exceeds the pre‑computed list (unlikely) */
    if ((idx_t)n > four_prime_cache_len)
        return four_prime_cache[four_prime_cache_len - 1];

    return four_prime_cache[(size_t)n - 1];  /* zero‑based index */
}

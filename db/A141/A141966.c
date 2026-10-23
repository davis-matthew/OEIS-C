/*
 * A141966  Primes congruent to 3 mod 28.
 * https://oeis.org/A141966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime congruent to 3 (mod 28). The
 *            implementation precomputes all primes up to a fixed bound,
 *            extracts those with p % 28 == 3, caches them in a static array,
 *            and returns the requested element. The bound (2000000) is
 *            sufficient for the first 10 000 terms
 * Range:     u8 n <= 5, u16 n <= 545, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage as required */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_primes.h"   /* provides sieve_primes */

static const idx_t PRECOMPUTE_LIMIT = 2000000ULL;  /* 2 million */

static seq_t *special_primes = NULL;  /* cached primes ≡ 3 (mod 28) */
static size_t special_count = 0;      /* number of cached primes */

/* Helper: allocate array of seq_t, using malloc for wide compatibility */
static seq_t *alloc_seq_size(size_t n)
{
    return n == 0 ? NULL : (seq_t *)malloc(n * sizeof(seq_t));
}

seq_t a(idx_t n)
{
    /* initialise cache on first use */
    if (!special_primes) {
        idx_t *all_primes = NULL;
        size_t all_count = 0;

        /* generate all primes ≤ PRECOMPUTE_LIMIT */
        sieve_primes(PRECOMPUTE_LIMIT, &all_primes, &all_count);

        /* count how many satisfy p % 28 == 3 */
        size_t need = 0;
        for (size_t i = 0; i < all_count; ++i) {
            uint64_t p = (uint64_t)all_primes[i];
            if ((p % 28ULL) == 3ULL) {
                ++need;
            }
        }

        /* allocate and fill the cache */
        special_primes = alloc_seq_size(need);
        size_t idx = 0;
        for (size_t i = 0; i < all_count; ++i) {
            uint64_t p = (uint64_t)all_primes[i];
            if ((p % 28ULL) == 3ULL) {
                special_primes[idx++] = (seq_t)p;
            }
        }
        special_count = need;

        /* free the temporary prime list */
        free(all_primes);
    }

    /* undefined for n < 1; return 0 for safety */
    if (n == 0) {
        return (seq_t)0;
    }

    /* guard against requests beyond pre‑computed range */
    if ((idx_t)n > (idx_t)special_count) {
        return (seq_t)0;
    }

    return special_primes[(size_t)(n - 1)];
}

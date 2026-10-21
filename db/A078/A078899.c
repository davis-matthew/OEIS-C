/*
 * A078899  Number of times the greatest prime factor of n is the greatest
 *          prime factor for numbers <=n; a(1)=1.
 * https://oeis.org/A078899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = #{k ≤ n : gpf(k) = gpf(n)} where gpf(k) is the greatest
 *            prime factor of k (gpf(1)=1)
 * Range:     u8 n <= 8683, u16 n <= 8683, u32 n <= 8683, u64 n <= 8683,
 *            u128 n <= 8683, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* Static state */
static seq_t *a_cache = NULL;          /* a(n) values, 1‑index: a_cache[0] = a(1) */
static size_t a_size = 0;              /* largest n computed so far */
static size_t a_cap  = 0;              /* capacity of a_cache */

static uint64_t *gpf_arr = NULL;       /* gpf_arr[i] = greatest prime factor of i */
static size_t gpf_cap  = 0;            /* capacity of gpf_arr */

/* Build gpf array up to 'limit' using a simple sieve and recompute a_cache.
   The routine is called only for increasing limits; for n ≤ 10 000 the
   cost is negligible. */
static void ensure_up_to(idx_t limit)
{
    if (limit <= a_size) return;              /* nothing to do */

    /* Ensure gpf_arr big enough and zeroed. */
    size_t new_cap = (size_t)limit + 1;     /* indices 0..limit */
    if (gpf_cap < new_cap) {
        uint64_t *tmp = (uint64_t*)realloc(gpf_arr, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        /* Zero new elements only; old ones will be overwritten. */
        if (gpf_cap == 0) {
            memset(tmp, 0, new_cap * sizeof(uint64_t));
        } else {
            memset(tmp + gpf_cap, 0,
                   (new_cap - gpf_cap) * sizeof(uint64_t));
        }
        gpf_arr = tmp;
        gpf_cap = new_cap;
    }
    memset(gpf_arr, 0, new_cap * sizeof(uint64_t));

    /* Sieve to compute greatest prime factor for each integer. */
    for (uint64_t p = 2; p <= (uint64_t)limit; ++p) {
        if (gpf_arr[p] == 0) {            /* p is prime */
            for (uint64_t m = p; m <= (uint64_t)limit; m += p) {
                gpf_arr[m] = p;           /* last (largest) prime that divides m */
            }
        }
    }
    gpf_arr[1] = 1;                       /* by definition */

    /* Allocate or grow a_cache. */
    size_t cache_needed = (size_t)limit;  /* a_cache[0 .. limit-1] */
    if (a_cap < cache_needed) {
        seq_t *tmp2 = (seq_t*)realloc(a_cache, cache_needed * sizeof(seq_t));
        if (!tmp2) exit(EXIT_FAILURE);
        a_cache = tmp2;
        a_cap  = cache_needed;
    }

    /* Count numbers with each greatest prime factor. */
    uint64_t *cnt = (uint64_t*)calloc(new_cap, sizeof(uint64_t));
    if (!cnt) exit(EXIT_FAILURE);

    for (size_t i = 1; i <= (size_t)limit; ++i) {
        uint64_t p = gpf_arr[i];
        ++cnt[p];
        a_cache[i - 1] = (seq_t)cnt[p];
    }

    free(cnt);
    a_size = limit;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* undefined for n < 1 */
    ensure_up_to(n);
    return a_cache[(size_t)n - 1];
}

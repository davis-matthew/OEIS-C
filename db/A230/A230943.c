/*
 * A230943  Number of (n+3) X (2+3) 0..3 white square subarrays x(i,j) with
 *          each element diagonally or antidiagonally next to at least one
 *          element with value (x(i,j)+1) mod 4, and upper left element zero.
 * https://oeis.org/A230943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence (from OEIS entry A230943):
 * a(n) = 31*a(n-2) + 375*a(n-4) + 233*a(n-6)
 *        - 7946*a(n-8) - 884*a(n-10) - 16296*a(n-12)
 *        - 203520*a(n-14) + 53120*a(n-16)
 * Initial values:
 *  a(1)=16; a(2)=34; a(3)=724; a(4)=1994; a(5)=28128; a(6)=76682;
 *  a(7)=1144652; a(8)=3107746; a(9)=46058744; a(10)=125263570;
 *  a(11)=1857863476; a(12)=5050618938; a(13)=74908095984;
 *  a(14)=203655079130; a(15)=3020446141788; a(16)=8211674722322;
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;      /* 1‑based cache of computed values */
static size_t cache_size = 0;    /* current capacity of 'cache' */
static idx_t computed_max = 0;   /* largest index that has been computed */

static const uint64_t init_vals[17] = {
    0ULL,         /* unused, a(0) */
    16ULL, 34ULL, 724ULL, 1994ULL, 28128ULL, 76682ULL, 1144652ULL,
    3107746ULL, 46058744ULL, 125263570ULL, 1857863476ULL,
    5050618938ULL, 74908095984ULL, 203655079130ULL,
    3020446141788ULL, 8211674722322ULL
};

/* Resize cache so that it can hold indices up to 'n' (inclusive). */
static void extend_cache(idx_t n)
{
    size_t need = (size_t)n + 1; /* indices 0 … n */
    if (need <= cache_size)
        return;
    seq_t *new_cache = seq_realloc(cache, cache_size, need);
    cache = new_cache;
    cache_size = need;
}

/* Compute all terms up to 'n', storing them in 'cache'. */
static void compute_upto(idx_t n)
{
    if (n <= computed_max)
        return;

    extend_cache(n);

    /* Ensure the initial 16 values are in place. */
    if (computed_max < 16) {
        idx_t start = computed_max + 1;
        if (start < 1) start = 1;
        for (idx_t i = start; i <= 16 && i <= n; ++i)
            cache[i] = (seq_t)init_vals[i];
    }

    /* Apply the recurrence for indices >= 17. */
    for (idx_t i = (computed_max > 16 ? computed_max + 1 : 17); i <= n; ++i) {
        seq_t term = (seq_t)31   * cache[i-2]
                   + (seq_t)375  * cache[i-4]
                   + (seq_t)233  * cache[i-6]
                   - (seq_t)7946 * cache[i-8]
                   - (seq_t)884  * cache[i-10]
                   - (seq_t)16296* cache[i-12]
                   - (seq_t)203520*cache[i-14]
                   + (seq_t)53120*cache[i-16];
        cache[i] = term;
    }

    computed_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* sequence is defined for n >= 1 */

    compute_upto(n);
    return cache[n];
}

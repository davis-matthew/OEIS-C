/*
 * A204691  Numbers n such that n contains exactly 5 digits, all distinct, and
 *          n^2 contains exactly 9 distinct digits.
 * https://oeis.org/A204691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   a(n) is the nth 5‑digit integer with all distinct digits for
 *            which its square has exactly 9 distinct decimal digits. Computed
 *            by a brute‑force scan of the interval [10000,99999]. The list is
 *            built once on first call and reused afterwards
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   distinct_five_digits, distinct_nine_digits_sq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/distinct_five_digits.h"
#include "../utils/distinct_nine_digits_sq.h"

static seq_t *cache = NULL;      /* array of found numbers                 */
static size_t cache_size = 0;    /* how many numbers are stored            */
static size_t cache_cap  = 0;    /* current capacity                       */

/* Build the cache on first use */
static void build_cache(void)
{
    if (cache != NULL)
        return;   /* already built */

    /* Initial capacity: 64, grow by factor 2 if necessary */
    size_t cap = 64;
    cache = (seq_t*)malloc(cap * sizeof(seq_t));
    if (!cache) abort();   /* out of memory */

    for (idx_t num = 10000; num <= 99999; ++num) {
        if (!distinct_five_digits(num))
            continue;

        uint64_t sq = (uint64_t)num * (uint64_t)num;
        if (!distinct_nine_digits_sq(sq))
            continue;

        /* Append to cache */
        if (cache_size == cap) {
            cap *= 2;
            seq_t *tmp = (seq_t*)realloc(cache, cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
        }
        cache[cache_size++] = (seq_t)num;
    }
}

seq_t a(idx_t n)
{
    /* Ensure the cache is populated */
    build_cache();

    /* Sequence is 1‑based; return 0 for out‑of‑range indices */
    if ((size_t)n == 0 || (size_t)(n - 1) >= cache_size)
        return (seq_t)0;

    return cache[n - 1];
}

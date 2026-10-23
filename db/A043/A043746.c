/*
 * A043746  Number of runs in the base 2 representation of n is congruent to 0
 *          mod 8.
 * https://oeis.org/A043746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose binary representation has
 *            a number of runs congruent to 0 (mod 8)
 * Range:     u8 n <= 1, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* cached values */
static size_t cache_len = 0;         /* number of cached values */
static size_t cache_cap = 0;         /* allocated capacity */
static idx_t last_tested = 0;        /* last integer examined */

static void grow_cache_to(size_t target)
{
    if (cache_cap < target) {
        size_t new_cap = cache_cap == 0 ? 256 : cache_cap * 2;
        if (new_cap < target) new_cap = target;
        seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = newp;
        cache_cap = new_cap;
    }
}

static void fill_cache_until(size_t target)
{
    while (cache_len < target) {
        ++last_tested;               /* next candidate integer */

        /* Count runs in the binary representation of last_tested. */
        idx_t x = last_tested;
        int runs = 0;
        int prev = -1;               /* no previous bit yet */

        while (x) {
            int bit = x & 1U;
            if (bit != prev) {
                ++runs;
                prev = bit;
            }
            x >>= 1;
        }

        if ((runs & 7U) == 0) {      /* runs % 8 == 0 */
            grow_cache_to(cache_len + 1);
            cache[cache_len++] = (seq_t)last_tested;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined case, but safe */
    fill_cache_until(n);
    return cache[n - 1];
}

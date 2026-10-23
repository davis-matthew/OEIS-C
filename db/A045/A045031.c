/*
 * A045031  Numbers whose base-4 representation contains exactly three 0's and
 *          three 1's.
 * https://oeis.org/A045031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   enumeration of base‑4 numbers with exactly three 0's and exactly
 *            three 1's
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* External types seq_t and idx_t are provided by the build system. */

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static idx_t next_len = 6;   /* minimal length that can contain 3 zeros and 3 ones */

/* enlarge the cache to hold at least 'needed' values */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);          /* out of memory – abort */
    cache = tmp;
    cache_cap = new_cap;
}

/* recursive generator that builds all base‑4 numbers of length L */
static void gen_len(idx_t L, idx_t pos,
                    idx_t zeros_left, idx_t ones_left,
                    idx_t more_free, seq_t val)
{
    if (pos == L) {
        if (zeros_left == 0 && ones_left == 0 && more_free == 0) {
            grow_cache(cache_size + 1);
            cache[cache_size++] = val;
        }
        return;
    }

    /* try digit 0 if allowed */
    if (zeros_left > 0 && !(pos == 0)) {
        gen_len(L, pos + 1, zeros_left - 1, ones_left, more_free, val * 4);
    }
    /* try digit 1 if allowed */
    if (ones_left > 0) {
        gen_len(L, pos + 1, zeros_left, ones_left - 1, more_free,
                val * 4 + 1);
    }
    /* try digits 2 and 3 if free slots remain */
    if (more_free > 0) {
        gen_len(L, pos + 1, zeros_left, ones_left, more_free - 1,
                val * 4 + 2);
        gen_len(L, pos + 1, zeros_left, ones_left, more_free - 1,
                val * 4 + 3);
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n=0, guard */

    while (cache_size < (size_t)n) {
        /* generate all numbers of the current length and then increase length */
        gen_len(next_len, 0, 3, 3, next_len - 6, 0);
        ++next_len;
    }
    return cache[n - 1];
}

/*
 * A039189  Numbers whose base-11 representation has the same number of 4's
 *          and 5's.
 * https://oeis.org/A039189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base-11 representation has equal number of 4’s and
 *            5’s
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static seq_t next_checked = 0;

static void grow_size(size_t min_cap)
{
    if (min_cap <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < min_cap) new_cap <<= 1;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

static void fill_cache_until(size_t target)
{
    while (cache_len < target) {
        seq_t num = next_checked;
        seq_t t = num;
        unsigned c4 = 0, c5 = 0;
        if (t == 0) {
            /* 0 has no 4 or 5 digits, counts stay 0 */
        } else {
            while (t != 0) {
                unsigned d = (unsigned)(t % (seq_t)11);
                if (d == 4) ++c4;
                else if (d == 5) ++c5;
                t /= 11;
            }
        }
        if (c4 == c5) {
            grow_size(cache_len + 1);
            cache[cache_len++] = num;
        }
        ++next_checked;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1, but protect */
    size_t target = (size_t)n;
    if (cache_len < target)
        fill_cache_until(target);
    return cache[target - 1];
}

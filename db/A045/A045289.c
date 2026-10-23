/*
 * A045289  Numbers whose base-5 representation contains exactly three 2's and
 *          no 4's.
 * https://oeis.org/A045289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   sequence of numbers whose base-5 representation contains exactly
 *            three 2's and no 4's
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static cache for already generated terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;
static uint64_t next_len = 3;     /* length to generate next */

/* enlarge cache if needed */
static void grow_cache(void)
{
    if (cache_size >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* OOM is fatal */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* push a value into the cache */
static void push_value(seq_t v)
{
    grow_cache();
    cache[cache_size++] = v;
}

/* recursive enumeration of all base‑5 numbers of a given length
   having exactly three digits '2' (no '4'). Digits are generated
   in lexicographic order so the resulting values are sorted. */
static void dfs_len(uint64_t pos, int twos_left, uint64_t value, uint64_t len)
{
    if (pos == len) {
        if (twos_left == 0) {
            push_value((seq_t)value);
        }
        return;
    }

    if (twos_left > (int)(len - pos)) return; /* impossible to finish */

    for (int d = 0; d <= 3; ++d) {
        if (pos == 0 && d == 0) continue;        /* leading zero not allowed */

        int new_twos = twos_left;
        if (d == 2) {
            if (twos_left == 0) continue;        /* too many twos */
            new_twos = twos_left - 1;
        }

        /* prune if remaining positions cannot accommodate the
           remaining number of twos */
        if (new_twos > 0 && (int)(len - pos - 1) < new_twos) continue;

        dfs_len(pos + 1, new_twos,
                value * 5 + (uint64_t)d, len);
    }
}

/* generate all numbers of a given length and append them to the cache */
static void generate_length(uint64_t len)
{
    dfs_len(0, 3, 0, len);
}

/* ensure that the cache contains at least 'target' terms */
static void ensure_cache(idx_t target)
{
    while (cache_size < (size_t)target) {
        generate_length(next_len);
        next_len++;                 /* next time generate longer numbers */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, return 0 as safety */
    ensure_cache(n);
    return cache[n - 1];
}

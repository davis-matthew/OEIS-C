/*
 * A218161  Recurring digital invariants of order 3.
 * https://oeis.org/A218161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) = minimal element of each cycle of length > 1 under mapping
 *            f(x) = sum of the cubes of the decimal digits of x
 * Range:     u8 none, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   alloc_seq_array, cube_digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/cube_digit_sum.h"

static seq_t *cycle_cache = NULL;
static size_t cycle_cache_len = 0;

/* build the cache of minimal cycle elements */
static void build_cache(void)
{
    if (cycle_cache != NULL)
        return;

    enum { MAX = 5832 };               /* maximum number that can appear in an orbit */
    bool processed[MAX + 1] = { false };
    bool min_in_cycle[MAX + 1] = { false };
    static idx_t pos_map[MAX + 1];
    idx_t path[MAX + 1];

    /* initialize position map */
    for (idx_t i = 0; i <= MAX; ++i)
        pos_map[i] = 0;

    for (idx_t start = 1; start <= MAX; ++start)
    {
        if (processed[start])
            continue;

        idx_t x = start;
        idx_t len = 0;

        /* follow the orbit until a repeat */
        while (true)
        {
            if (x > MAX)
                break;                     /* should never happen for the given range */

            if (pos_map[x] != 0)           /* x already in current path */
            {
                idx_t cycle_start = pos_map[x] - 1;
                idx_t cycle_len = len - cycle_start;
                if (cycle_len > 1)
                {
                    /* identify the minimal element of the cycle */
                    idx_t cmin = path[cycle_start];
                    for (idx_t j = cycle_start + 1; j < len; ++j)
                        if (path[j] < cmin)
                            cmin = path[j];
                    min_in_cycle[cmin] = true;
                }
                break;
            }

            if (processed[x])              /* reached a node that has been processed before */
            {
                break;
            }

            pos_map[x] = len + 1;          /* store index+1 to distinguish from unvisited */
            path[len++] = x;
            x = (idx_t)cube_digit_sum((seq_t)x);
        }

        /* mark all visited nodes as processed and clear pos_map entries */
        for (idx_t j = 0; j < len; ++j)
        {
            processed[path[j]] = true;
            pos_map[path[j]] = 0;
        }
    }

    /* count minimal cycle elements to allocate the cache */
    size_t cnt = 0;
    for (idx_t i = 1; i <= MAX; ++i)
        if (min_in_cycle[i])
            ++cnt;

    cycle_cache = alloc_seq_array(cnt);
    cycle_cache_len = cnt;
    size_t k = 0;
    for (idx_t i = 1; i <= MAX; ++i)
        if (min_in_cycle[i])
            cycle_cache[k++] = (seq_t)i;
}

/* sequence entry */
seq_t a(idx_t n)
{
    build_cache();
    if (n == 0 || n > cycle_cache_len)
        return 0;
    return cycle_cache[n - 1];
}

/*
 * A088101  Group the natural numbers with at least two members in each group
 *          such that the n-th group sum is a multiple of n. (1,2), (3,4,5),
 *          (6,7,8), (9,10,11,12,13,14,15), (16,17,18,19),
 *          (20,21,22,23,24,25,26,27,28), (29,30,31,32,33,34), ... Sequence
 *          contains the n-th group sum divided by n.
 * https://oeis.org/A088101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) is the sum of the n-th group of consecutive natural
 *            numbers * (with at least two members) divided by n, where each
 *            group is * the smallest block starting immediately after the
 *            previous one * whose sum is a multiple of n
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* All arithmetic uses seq_t. We keep a cache of already computed
 * values so that multiple calls are O(1).  The groups are built
 * lazily and on-demand.  The algorithm is linear in the number
 * of natural numbers examined, which is well below the limits
 * for n up to 10 000. */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed a(n) values, 0‑based */
static size_t cache_cap = 0;     /* capacity of cache array      */
static size_t cache_size = 0;    /* number of values stored      */

static idx_t next_number = 1;    /* next natural number to use   */
static idx_t cur_group = 1;      /* group index that we are about to build */

static void ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    while (new_cap < need)
        new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* In the unlikely event that realloc fails the program will
       terminate with a signal; this is acceptable for the
       competitive environment. */
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined, but avoid accessing cache[−1]   */
        return (seq_t)0;

    /* Return cached value if available. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Build successive groups until we have a(n). */
    while (cache_size < n)
    {
        seq_t sum = 0;
        idx_t len = 0;

        /* Find the smallest block starting at next_number
           whose sum is a multiple of cur_group and
           contains at least two numbers. */
        while (1)
        {
            sum += next_number;
            ++len;
            ++next_number;

            if (len >= 2 && (sum % (seq_t)cur_group) == 0)
                break;
        }

        /* Store the value a(cur_group) = sum / cur_group. */
        ensure_cache(cache_size + 1);
        cache[cache_size] = sum / (seq_t)cur_group;
        ++cache_size;
        ++cur_group;
    }

    return cache[n - 1];
}

/*
 * A172173  Sums of NE-SW diagonals of triangle A172171.
 * https://oeis.org/A172173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1; If n is even: a(n)=a(n-1)+a(n-2); If n is odd :
 *            a(n)=a(n-1)+a(n-2)+9
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 182,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 998 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* allocated capacity */
    static idx_t max_n = (idx_t)-1;   /* last computed index */

    /* Lazy initialization */
    if (max_n == (idx_t)-1)
    {
        grow_seq_array(&cache, &cache_cap, 2);   /* allocate indices 0..1 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        max_n   = 1;
    }

    if (n <= max_n)
        return cache[n];

    /* Ensure capacity up to n */
    if (cache_cap < (size_t)(n + 1))
        grow_seq_array(&cache, &cache_cap, (size_t)(n + 1));

    /* Compute missing terms */
    for (idx_t i = max_n + 1; i <= n; ++i)
    {
        if ((i & 1) == 0) /* i even */
            cache[i] = cache[i - 1] + cache[i - 2];
        else              /* i odd */
            cache[i] = cache[i - 1] + cache[i - 2] + (seq_t)9;
    }

    max_n = n;
    return cache[n];
}

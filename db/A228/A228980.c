/*
 * A228980  Number of n X 2 0..1 arrays of the median of the corresponding
 *          element, the element to the east and the element to the south in a
 *          larger (n+1) X 3 0..1 array.
 * https://oeis.org/A228980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2) - 6*a(n-3) + 6*a(n-4); a(1)=4,
 *            a(2)=16, a(3)=64, a(4)=252
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Ensure that *arr has capacity at least needed entries. */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **arr)
{
    if (needed <= *cap) return;
    seq_t *p = (seq_t *)realloc(*arr, needed * sizeof(seq_t));
    if (!p) exit(EXIT_FAILURE);
    *arr = p;
    *cap = needed;
}

seq_t a(idx_t n)
{
    /* Handle the first four values directly. */
    if (n <= 4)
    {
        switch ((size_t)n)
        {
            case 1: return (seq_t)4;
            case 2: return (seq_t)16;
            case 3: return (seq_t)64;
            case 4: return (seq_t)252;
            default: return (seq_t)0;  /* n < 1 (should not occur) */
        }
    }

    static seq_t *cache = NULL;   /* array storing computed terms */
    static size_t capacity = 0;   /* current allocation size */
    static size_t computed = 0;   /* largest n for which cache[n] is valid */

    size_t sn = (size_t)n;        /* use size_t for indexing */

    /* Make sure the array is large enough. */
    ensure_capacity(sn + 1, &capacity, &cache);

    /* Initialize the first four terms if they are not yet set. */
    if (computed < 4)
    {
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)16;
        cache[3] = (seq_t)64;
        cache[4] = (seq_t)252;
        computed = 4;
    }

    /* Compute terms up to the requested n. */
    for (size_t i = computed + 1; i <= sn; ++i)
    {
        cache[i] = 6*cache[i-1] - 7*cache[i-2] - 6*cache[i-3] + 6*cache[i-4];
    }
    computed = sn;

    return cache[sn];
}

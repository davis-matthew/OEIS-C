/*
 * A219405  Unmatched value maps: number of n X 3 binary arrays indicating the
 *          locations of corresponding elements not equal to any horizontal or
 *          antidiagonal neighbor in a random 0..2 n X 3 array.
 * https://oeis.org/A219405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 15*a(n-2) + 9*a(n-3); Initial values: a(1)=4,
 *            a(2)=28, a(3)=181
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for previously computed terms (1‑based indexing). */
static seq_t *cache      = NULL;   /* array of values, cache[0] unused */
static idx_t cache_len   = 0;      /* highest index that has a valid value */
static size_t cache_cap  = 0;      /* allocated size of the array (in terms of indices) */

/* Ensure the cache can store indices up to “needed”. */
static void ensure_capacity(idx_t needed)
{
    /* needed+1 indices including the zero entry. */
    size_t required = (size_t)(needed + 1);
    if (required <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (required > new_cap)
        new_cap = required;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) {
        /* In a real program we would handle allocation failure.  For the
           purposes of these tests an out‑of‑memory condition is not
           expected for n up to 10 000. */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of A219405. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n<1 */

    if (n <= cache_len)
        return cache[n];

    ensure_capacity(n);

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 1)
            cache[i] = (seq_t)4;
        else if (i == 2)
            cache[i] = (seq_t)28;
        else if (i == 3)
            cache[i] = (seq_t)181;
        else
            cache[i] = cache[i - 1] * (seq_t)4
                     + cache[i - 2] * (seq_t)15
                     + cache[i - 3] * (seq_t)9;
    }
    cache_len = n;
    return cache[n];
}

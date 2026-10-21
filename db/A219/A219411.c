/*
 * A219411  Unmatched value maps: number of 2 X n binary arrays indicating the
 *          locations of corresponding elements not equal to any horizontal or
 *          antidiagonal neighbor in a random 0..2 2 X n array.
 * https://oeis.org/A219411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) + 6*a(n-3) - 3*a(n-4) + 4*a(n-5) (for
 *            n ≥ 6), with initial terms a(1)=1, a(2)=7, a(3)=28, a(4)=98,
 *            a(5)=351
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* array of cached terms, 0‑based */
static idx_t cache_len = 0;   /* number of terms currently cached */

/* Ensure terms 1 … n are computed and stored in cache[0 … n-1] */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len)            /* already cached */
        return;

    /* First time: create space for the 5 known initial terms */
    if (cache_len == 0) {
        idx_t const base_len = 5;
        cache = (seq_t*)malloc(base_len * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)28;
        cache[3] = (seq_t)98;
        cache[4] = (seq_t)351;
        cache_len = base_len;
        if (n <= cache_len)
            return;
    }

    /* Grow the cache to hold n terms */
    seq_t *tmp = (seq_t*)realloc(cache, n * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* Compute terms from cache_len+1 up to n */
    for (idx_t i = cache_len; i < n; ++i) {
        /* i is 0‑based; term number is i+1 */
        seq_t t1 = (seq_t)4 * cache[i-1];
        seq_t t2 = (seq_t)3 * cache[i-2];
        seq_t t3 = (seq_t)6 * cache[i-3];
        seq_t t4 = (seq_t)3 * cache[i-4];
        seq_t t5 = (seq_t)4 * cache[i-5];
        cache[i] = t1 - t2 + t3 - t4 + t5;
    }
    cache_len = n;
}

/* Return the n-th term of A219411 (1‑based indexing) */
seq_t a(idx_t n)
{
    if (n == 0)           /* the sequence is defined for n≥1, return 0 for safety */
        return (seq_t)0;
    ensure_cache(n);
    return cache[n-1];
}

/*
 * A145730  Partial sums of A108019.
 * https://oeis.org/A145730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 4*(8^(n+1)-7*n-8)/49
 * Recurrence: a(0)=0, a(1)=4, a(2)=40,
 *   a(n)=10*a(n-1)-17*a(n-2)+8*a(n-3)  (n≥3)
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Cached sequence values */
static seq_t *a_cache = NULL;   /* pointer to array of seq_t values */
static size_t a_cap   = 0;       /* current allocated capacity (number of elements) */
static idx_t   a_last  = 0;      /* index of last computed element in cache */

/* Initialise cache with first three terms */
static void a_init(void)
{
    a_cap  = 3;
    a_cache = alloc_seq_array(a_cap);
    a_cache[0] = (seq_t)0;
    a_cache[1] = (seq_t)4;
    a_cache[2] = (seq_t)40;
    a_last  = 2;
}

/* Ensure the cache can hold indices up to n (inclusive) */
static void a_ensure_cap(idx_t n)
{
    size_t new_cap = (size_t)n + 1;
    if (a_cap >= new_cap)
        return;

    seq_t *new_cache = alloc_seq_array(new_cap);
    /* copy existing values */
    for (size_t i = 0; i < a_cap; ++i)
        new_cache[i] = a_cache[i];

    free_seq_array(a_cache);
    a_cache = new_cache;
    a_cap   = new_cap;
}

/* Extend cache to compute all terms up to n */
static void a_compute(idx_t n)
{
    if (a_cache == NULL)
        a_init();

    if (a_last >= n)
        return;   /* already computed */

    a_ensure_cap(n);

    for (idx_t i = a_last + 1; i <= n; ++i) {
        seq_t t1 = a_cache[i - 1];
        seq_t t2 = a_cache[i - 2];
        seq_t t3 = a_cache[i - 3];
        a_cache[i] = t1 * (seq_t)10 - t2 * (seq_t)17 + t3 * (seq_t)8;
    }
    a_last = n;
}

seq_t a(idx_t n)
{
    a_compute(n);
    return a_cache[n];
}

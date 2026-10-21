/*
 * A097112  Expansion of (1+4x-6x^2-36x^3)/(1-19x^2+90x^4).
 * https://oeis.org/A097112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=4, a(2)=13, a(3)=40 * a(n) = 19*a(n-2) - 90*a(n-4)
 *            for n ≥ 4
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
 
/* Cached values for successive n.
 * The array grows as needed; values are stored for all indices
 * 0 ≤ i ≤ cache_max.  The array is static so that successive
 * calls can reuse previous computations. */
static seq_t *cache = NULL;        /* pointer to allocated array */
static idx_t   cache_max = 0;      /* largest index actually stored */

/* Ensure the cache can hold at least indices 0..n.
 * If the array is uninitialized, allocate it.
 * If the desired size exceeds current capacity, grow using realloc. */
static void
ensure_cache_size(idx_t n)
{
    if (cache && cache_max > n)
        return;          /* already large enough */

    /* Allocate new size; double growth to keep amortized O(1) */
    idx_t new_size = (cache_max ? cache_max * 2 : 16);
    if (new_size < n + 1)
        new_size = n + 1;

    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!cache) {
        /* Allocation failed; terminate program (no output is expected). */
        abort();
    }

    /* Initialize newly allocated entries to zero.  (seq_t may be a class
     * type; the default constructor is invoked for the new part.) */
    for (idx_t i = cache_max; i < new_size; ++i)
        cache[i] = (seq_t)0;

    cache_max = new_size - 1;
}

/* Initialize the known base values if they have not yet been set.  */
static void
init_base_values(void)
{
    if (cache[0] == (seq_t)0) cache[0] = (seq_t)1;
    if (cache[1] == (seq_t)0) cache[1] = (seq_t)4;
    if (cache[2] == (seq_t)0) cache[2] = (seq_t)13;
    if (cache[3] == (seq_t)0) cache[3] = (seq_t)40;
}

/* Return the a(n) value for the sequence A097112. */
seq_t a(idx_t n)
{
    /* Ensure we have space for all indices up to n. */
    ensure_cache_size(n);

    /* Set base values if they are missing. */
    init_base_values();

    /* If the value is already computed, return it. */
    if (cache[n] != (seq_t)0)
        return cache[n];

    /* Compute missing values iteratively from the smallest missing
     * index up to n.  The recurrence only depends on indices
     * n-2 and n-4, so previous values are guaranteed to exist. */
    for (idx_t i = 4; i <= n; ++i) {
        if (cache[i] != (seq_t)0) continue;        /* already computed */

        seq_t v = (seq_t)19 * cache[i - 2] - (seq_t)90 * cache[i - 4];
        cache[i] = v;
    }

    return cache[n];
}

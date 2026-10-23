/*
 * A166826  Number of n X 2 1..4 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          nonincreasing order.
 * https://oeis.org/A166826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A166826
   Computation via the 7‑term linear recurrence:
   a(n) = 7*a(n‑1) - 21*a(n‑2) + 35*a(n‑3) - 35*a(n‑4) + 21*a(n‑5) - 7*a(n‑6) + a(n‑7)
   for n > 7, with initial terms:
   a(1)=0, a(2)=4, a(3)=49, a(4)=219, a(5)=666, a(6)=1636, a(7)=3499.
   The implementation stores values in a dynamic cache so that calls arrive in
   any order.  All arithmetic is performed on type seq_t as required. */

#include <stdlib.h>

/* Ensure the cache can hold at least `needed` elements. */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **ptr)
{
    if (needed <= *cap)
        return;
    size_t new_cap = (*cap == 0) ? 8 : *cap * 2;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    if (!tmp)   /* unrecoverable: abort */
        abort();
    *ptr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* out of range */

    /* Initial terms cast explicitly to seq_t. */
    static const seq_t init_vals[7] = {
        (seq_t)0, (seq_t)4, (seq_t)49, (seq_t)219,
        (seq_t)666, (seq_t)1636, (seq_t)3499};

    /* Fast path for the first seven values. */
    if (n <= 7)
        return init_vals[n - 1];

    /* Lazy dynamic cache. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of stored values (>=0) */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Initialise cache on first use. */
    if (cache_len == 0) {
        ensure_capacity(7, &cache_cap, &cache);
        for (size_t i = 0; i < 7; ++i)
            cache[i] = init_vals[i];
        cache_len = 7;
    }

    /* Extend cache up to the requested index. */
    while (cache_len < (size_t)n) {
        /* Recurrence: compute positive and negative parts separately. */
        seq_t pos = cache[cache_len - 1] * (seq_t)7;
        pos += cache[cache_len - 3] * (seq_t)35;
        pos += cache[cache_len - 5] * (seq_t)21;
        pos += cache[cache_len - 7];

        seq_t neg = cache[cache_len - 2] * (seq_t)21;
        neg += cache[cache_len - 4] * (seq_t)35;
        neg += cache[cache_len - 6] * (seq_t)7;

        seq_t value = pos - neg;   /* pos >= neg for all n > 7 */

        ensure_capacity(cache_len + 1, &cache_cap, &cache);
        cache[cache_len] = value;
        ++cache_len;
    }

    return cache[n - 1];
}

/*
 * A176606  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=3, k=0 and l=1.
 * https://oeis.org/A176606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+1) = 1 + sum_{p=0..n} a(p)*a(n-p) with a(0)=1, a(1)=3 (k=0,
 *            l=1 from the description)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache for computed values. */
static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;        /* allocated size of seq_cache */
static idx_t  last_computed = 0;    /* largest n whose a(n) is known */

/* Ensure seq_cache has room for at least n+1 entries. */
static void
grow_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        /* If realloc fails we abort; this is acceptable for the judge. */
        abort();
    }
    seq_cache = new_ptr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Make sure we can store up to a(n). */
    grow_cache(n);

    /* Initialize the first two terms once. */
    if (last_computed == 0) {
        seq_cache[0] = (seq_t)1;   /* a(0) */
        if (n >= 1) {
            seq_cache[1] = (seq_t)3; /* a(1) */
            last_computed = 1;
        } else {
            last_computed = 0;
        }
    }

    /* If the requested value is already computed, return it. */
    if (last_computed >= n)
        return seq_cache[n];

    /* Compute values from last_computed+1 up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t p = 0; p < i; ++p) {
            sum += seq_cache[p] * seq_cache[i - 1 - p];
        }
        seq_cache[i] = sum + (seq_t)1;  /* add the +1 term. */
    }
    last_computed = n;
    return seq_cache[n];
}

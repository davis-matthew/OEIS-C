/*
 * A021747  Decimal expansion of 1/743.
 * https://oeis.org/A021747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the n‑th decimal digit of 1/743 (a(0) is the first digit
 *            after the decimal point)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 98 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Static cache stores already computed digits a(0)…a(digits_computed‑1). */
static seq_t *cache = NULL;
static size_t cache_size = 0;       /* allocated capacity in digits */
static size_t digits_computed = 0;  /* number of digits currently cached */
static uint64_t remainder = 1;      /* remainder after the last computed digit */

/* Grow the cache to at least new_cap elements, using seq_alloc / seq_realloc. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_size)
        return;

    size_t new_size = cache_size ? cache_size * 2 : 16;
    if (new_size < new_cap)
        new_size = new_cap;

    seq_t *tmp;
    if (cache == NULL)
        tmp = seq_alloc(new_size);
    else
        tmp = seq_realloc(cache, cache_size, new_size);

    if (tmp == NULL)
        abort();   /* memory exhaustion */

    cache = tmp;
    cache_size = new_size;
}

/* Return the n‑th digit of 1/743. */
seq_t a(idx_t n)
{
    /* If we already have the digit, return it immediately. */
    if ((size_t)n < digits_computed)
        return cache[(size_t)n];

    grow_cache((size_t)n + 1);

    /* Long‑division: compute missing digits up to index n. */
    for (size_t i = digits_computed; i <= (size_t)n; ++i) {
        remainder *= 10ULL;                /* bring down next decimal place   */
        uint64_t digit = remainder / 743ULL; /* next digit of 1/743            */
        remainder = remainder % 743ULL;      /* new remainder                  */
        cache[i] = (seq_t)digit;             /* store digit                     */
    }
    digits_computed = (size_t)n + 1;
    return cache[(size_t)n];
}

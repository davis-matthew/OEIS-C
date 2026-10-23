/*
 * A189730  Partial sums of A189727.
 * https://oeis.org/A189730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   s(n) = s(floor((n+2)/3)) + floor((n+1)/3) + floor(n/3) -
 *            s(floor(n/3)), with s(0)=0, s(1)=0. The function a returns s(n)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;                   /* Stored values of s(n)             */
static unsigned char *computed = NULL;        /* Flags: 1 if cache[n] is valid     */
static idx_t cache_cap = 0;                   /* Size of allocated cache (n+1)    */

/* Ensure that cache is large enough for indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;

    idx_t new_cap = n + 1;
    void *ptr = realloc(cache, new_cap * sizeof(seq_t));
    if (!ptr)
        abort();
    cache = (seq_t *)ptr;

    ptr = realloc(computed, new_cap * sizeof(unsigned char));
    if (!ptr)
        abort();
    computed = (unsigned char *)ptr;

    /* Initialise the newly allocated slots. */
    for (idx_t i = cache_cap; i < new_cap; ++i) {
        cache[i] = 0;
        computed[i] = 0;
    }
    cache_cap = new_cap;

    /* Base values. */
    cache[0] = 0;
    computed[0] = 1;
    if (cache_cap > 1) {
        cache[1] = 0;
        computed[1] = 1;
    }
}

/* Recursive evaluation of s(n) with memoization. */
static seq_t s_rec(idx_t n)
{
    ensure_cache(n);
    if (computed[n])
        return cache[n];

    idx_t a = (n + 2) / 3;
    idx_t b = n / 3;
    seq_t part1 = s_rec(a);          /* s(floor((n+2)/3)) */
    seq_t part2 = s_rec(b);          /* s(floor(n/3))     */
    seq_t val = part1
                + (seq_t)((n + 1) / 3)
                + (seq_t)(n / 3)
                - part2;

    cache[n] = val;
    computed[n] = 1;
    return val;
}

/* External entry point: returns the nth term of A189730 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* defensive: offset is 1 */
    return s_rec(n);
}

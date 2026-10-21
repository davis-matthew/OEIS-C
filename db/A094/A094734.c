/*
 * A094734  Number of connected 2-element multiantichains on a labeled n-set.
 * https://oeis.org/A094734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5 - 3*2^n + 3^n)/2 for n>0 Recurrence: a(0)=0, a(1)=1,
 *            a(2)=1, a(3)=4, a(n)=6*a(n-1)-11*a(n-2)+6*a(n-3) for n>3. The
 *            function memoizes values so it works for calls in arbitrary
 *            order
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>


static seq_t *cache = NULL;       /* array holding computed terms          */
static idx_t cache_len = 0;       /* allocated length (>=1)                 */
static idx_t computed_max = 0;    /* largest index that has been computed   */

/* Ensure that the cache can hold indices up to n (inclusive).   */
static void ensure_capacity(idx_t n)
{
    if (cache_len > n) return;
    idx_t new_len = n + 1;
    if (new_len < 16) new_len = 16;    /* initial block size */
    new_len *= 2;                      /* overshoot for future */
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) exit(1);                 /* abort if allocation fails */
    cache = tmp;
    cache_len = new_len;
}

/* Compute all terms up to n (if not already done). */
static void compute_up_to(idx_t n)
{
    if (n <= computed_max) return;
    ensure_capacity(n);

    /* Initialise the base values needed for the recurrence. */
    if (computed_max < 3) {
        if (0 <= n) cache[0] = (seq_t)0;
        if (1 <= n) cache[1] = (seq_t)1;
        if (2 <= n) cache[2] = (seq_t)1;
        if (3 <= n) cache[3] = (seq_t)4;
        computed_max = (n >= 3) ? 3 : n;  /* highest computed index so far */
    }

    /* Extend using the linear recurrence. */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        cache[i] = cache[i-1] * (seq_t)6
                 - cache[i-2] * (seq_t)11
                 + cache[i-3] * (seq_t)6;
    }
    computed_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    compute_up_to(n);
    return cache[n];
}

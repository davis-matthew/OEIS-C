/*
 * A101361  a(1) = a(2) = 1; for n > 2, a(n) = Knuth's Fibonacci (or circle)
 *          product "a(n-1) o a(n-2)".
 * https://oeis.org/A101361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2*Fibonacci(n)) Third‑order recurrence: a(1) =
 *            1, a(2) = 1, a(3) = 3 a(n) = (a(n-1)^2 – a(n-2)^2) / a(n-3) for
 *            n ≥ 4
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* 1‑based cache of computed terms      */
static idx_t cache_size = 0;     /* highest index currently cached       */

/* Extend the cache to hold terms up to index n and compute any missing values. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;                              /* already have the needed terms          */

    /* Allocate space for indices 0 … n  (index 0 unused).  */
    seq_t *newp = (seq_t *)realloc(cache, (n + 1u) * sizeof(seq_t));
    if (!newp)                                 /* allocation failure – abort           */
        exit(EXIT_FAILURE);
    cache = newp;

    /* Initialise the first three values if they are not already ready.   */
    if (cache_size < 1 && n >= 1) cache[1] = (seq_t)1;
    if (cache_size < 2 && n >= 2) cache[2] = (seq_t)1;
    if (cache_size < 3 && n >= 3) cache[3] = (seq_t)3;

    /* Compute from the first missing index up to n.  Start at 4, because
       the recurrence uses three previous terms.                                 */
    idx_t start = cache_size + 1;
    if (start < 4) start = 4;
    for (idx_t i = start; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t tmp = a1 * a1 - a2 * a2;  /* a(n‑1)^2 – a(n‑2)^2    */
        tmp /= a3;                      /* divide by a(n‑3)       */
        cache[i] = tmp;
    }
    cache_size = n;                     /* remember the new maximum index     */
}

/* External linkage:  seq_t a(idx_t n)  */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                  /* undefined for n < 1 – safe return */

    ensure_cache(n);
    return cache[n];
}

/*
 * A174851  y-values in the solution to x^2-46*y^2=1.
 * https://oeis.org/A174851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 48670*a(n-1) - a(n-2) (a(1) = 0, a(2) = 3588)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a static cache up to a hard‑coded
   maximum index (MAX_IDX). It works for both primitive integer
   types and for an arbitrary‑precision seq_t class, because only
   basic arithmetic operators are used. */

/* Adjust MAX_IDX if you need to support larger n. */
#define MAX_IDX 10000

static seq_t cache[MAX_IDX + 1]; /* cache[1] stores a(1) */
static idx_t cache_len = 0;      /* number of terms already computed */

/* Compute terms up to desired index n (1‑based). */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;                /* already have them */

    /* Guard against out‑of‑range requests. */
    if (n > MAX_IDX)                          /* clamp to MAX_IDX */
        n = MAX_IDX;

    /* Initialise the first two terms if necessary. */
    if (cache_len == 0) {
        cache[1] = (seq_t)0;                  /* a(1) */
        cache_len = 1;
    }
    if (cache_len == 1) {
        cache[2] = (seq_t)3588;               /* a(2) */
        cache_len = 2;
    }

    /* Populate remaining terms. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        cache[i] = (seq_t)48670 * cache[i - 1] - cache[i - 2];
    }

    cache_len = n;
}

/* Public entry point. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined for 0 or negative indices */
    if (n > MAX_IDX)           /* handle out‑of‑range: compute only up to MAX_IDX */
        n = MAX_IDX;
    ensure_cache(n);
    return cache[n];
}

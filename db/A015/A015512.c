/*
 * A015512  a(1) = 1, a(n) = Sum_{k=1..n-1} ((10^k - 1)/9)*a(k).
 * https://oeis.org/A015512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A015512:
 *  a(1) = 1
 *  a(2) = 1
 *  a(n) = sum_{k=1}^{n-1} ((10^k-1)/9)*a(k)
 *  a(n) = ((10^(n-1)+8)/9) * a(n-1)   (for n >= 3)
 *  A recurrence factor f_{i+1} = 10*f_i - 8  with f_2 = 12
 *  (f_i = (10^i + 8)/9 for i >= 2)
 */

#include <stdlib.h> /* size_t, malloc, realloc */
#include <stddef.h> /* NULL */

static seq_t *cache      = NULL;   /* array of computed a(n), zero‑based */
static size_t cache_len  = 0;       /* number of elements stored (a(1) at cache[0]) */
static size_t cache_cap  = 0;       /* allocated capacity of the cache */
static seq_t f_next      = 0;       /* factor for the next term (f_i) */

/* enlarge the cache to hold at least new_cap elements */
static void ensure_cap(size_t new_cap)
{
    if (cache_cap >= new_cap)
        return;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)         /* allocation failed – keep old state */
        return;
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;          /* undefined for n < 1 */

    size_t N = (size_t)n; /* cast once, use size_t for indexing */

    /* make room for up to n elements */
    ensure_cap(N);
    if (cache_cap < N)
        return (seq_t)0;          /* allocation failed – give what we have */

    /* initialise the first terms if missing */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;      /* a(1) */
        cache_len = 1;
    }
    if (cache_len == 1 && n >= (size_t)2) {
        cache[1] = (seq_t)1;      /* a(2) */
        cache_len = 2;
        f_next = (seq_t)12;       /* factor for n = 3 (f_2) */
    }

    /* compute any missing terms up to a(n) */
    /* cache_len <= n-1 at this point (n>=3) or already equal */
    for (size_t i = cache_len + 1; i <= N; ++i) {
        /* i is the index of the term to compute (1‑based) */
        cache[i - 1] = f_next * cache[i - 2];   /* a(i) = f_next * a(i-1) */
        /* update factor for the next term */
        f_next = (seq_t)10 * f_next - (seq_t)8;
    }
    cache_len = N;

    return cache[N - 1];
}

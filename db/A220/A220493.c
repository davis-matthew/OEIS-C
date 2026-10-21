/*
 * A220493  Fibonacci 15-step numbers, a(n) = a(n-1) + a(n-2) + ... + a(n-15).
 * https://oeis.org/A220493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + ... + a(n-15) initial values: a(1)=1,
 *            a(n)=2^(n-2) for 2<=n<=15
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;    /* 1‑based indices stored at cache[i-1] */
static size_t cap   = 0;        /* allocated capacity */
static idx_t stored = 0;        /* number of terms currently stored */

/* Allocate or grow the cache to hold at least 'need' elements */
static void grow_cache(size_t need)
{
    if (need <= cap) return;
    size_t newcap = cap ? cap : 16;
    while (newcap < need) newcap <<= 1;
    seq_t *ptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache = ptr;
    cap   = newcap;
}

/* Ensure the first 15 terms are stored */
static void init_first15(void)
{
    if (stored >= 15) return;
    grow_cache(15);
    cache[0] = 1;                     /* a(1) */
    for (idx_t i = 2; i <= 15; ++i)
        cache[i-1] = (seq_t)1 << (i-2); /* a(i) for i>=2 */
    stored = 15;
}

/* Compute a(n) using memoised recurrence when n > 15 */
static void compute_range(idx_t n)
{
    if (stored >= n) return;          /* already computed */
    if (stored < 15) init_first15();   /* make sure base terms exist */

    grow_cache(n);
    for (idx_t i = stored + 1; i <= n; ++i) {
        seq_t sum = 0;
        idx_t start = (i > 15) ? (i - 15) : 1;
        for (idx_t j = start; j < i; ++j)
            sum += cache[j-1];
        cache[i-1] = sum;
    }
    stored = n;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n <= 15) return (seq_t)1 << (n-2);
    compute_range(n);
    return cache[n-1];
}

/*
 * A050072  a(n) = |a(n-1) - a(m)| for n >= 4, where m = 2^(p+1) + 2 - n and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = a(2) = a(3) = 1.
 * https://oeis.org/A050072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=a(2)=a(3)=1; for n>=4: a(n) = |a(n-1) - a(m)| where p =
 *            floor(log2(n-2)), m = 2^(p+1) + 2 - n
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Build uses seq_t and idx_t types defined by the test harness. */
#include <stdlib.h>

/* Simple floor(log2) for unsigned 64‑bit integers. */
static unsigned floor_log2_uint64(idx_t x)
{
    unsigned p = 0;
    while (x >>= 1) ++p;
    return p;
}

/* Cache for computed values. */
static seq_t *cache      = NULL; /* array indexed from 1 */
static idx_t cache_size  = 0;    /* allocated elements (including index 0) */
static idx_t computed_up = 0;    /* largest n for which cache[i] is valid */

/* Ensure cache contains values up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n <= computed_up) return;          /* already computed */

    /* Grow the underlying array if needed. */
    if ((idx_t)cache_size <= n) {
        idx_t new_size = (n + 1) * 2;       /* grow by factor 2 */
        if (new_size < 64) new_size = 64;   /* minimal size */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);      /* out of memory */
        cache     = tmp;
        cache_size = new_size;
    }

    /* Compute new entries. */
    for (idx_t i = computed_up + 1; i <= n; ++i) {
        if (i <= 3) {                  /* initial values */
            cache[i] = (seq_t)1;
        } else {
            /* Determine p and m for this i. */
            unsigned p   = floor_log2_uint64(i - 2);        /* p = floor(log2(i-2)) */
            idx_t pow2   = ((idx_t)1) << (p + 1);          /* 2^(p+1) */
            idx_t m      = pow2 + (idx_t)2 - i;           /* m = 2^(p+1)+2-i */

            seq_t a1 = cache[i - 1];
            seq_t am = cache[m];                         /* m < i, already computed */

            cache[i] = (a1 >= am) ? (a1 - am) : (am - a1); /* |a1 - am| */
        }
    }
    computed_up = n;
}

extern seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* sequence defined only for n>=1 */
    ensure_cache(n);
    return cache[n];
}

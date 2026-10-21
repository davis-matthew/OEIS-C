/*
 * A050060  a(n) = a(n-1) + a(m) for n >= 4, where m = 2*n - 3 - 2^(p+1) and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = 1, a(2) = 3, and a(3) = 2.
 * https://oeis.org/A050060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=3, a(3)=2; for n>=4, a(n)=a(n-1)+a(m) where m =
 *            2*n-3-2^(p+1) and p = floor(log2(n-2))
 * Range:     u8 n <= 22, u16 n <= 57, u32 n <= 195, u64 n <= 770,
 *            u128 n <= 3633, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Memoization cache */
static seq_t *cache = NULL;      /* array of a(n) values, index starts at 1 */
static size_t cache_cap = 0;     /* allocated capacity (>= cache_len+1) */
static size_t cache_len = 0;     /* largest index computed (>=0) */

/* Ensure the cache can store values up to given n (1‑based). */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n;
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need)
        new_cap *= 2;

    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        /* In a contest setting we abort on allocation failure. */
        abort();
    }
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Compute a(n) with memoization. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;    /* undefined for n<1, return 0 as safety */

    /* If result already cached, return it. */
    if (n <= cache_len)
        return cache[(size_t)n];

    /* Ensure cache size. */
    ensure_cache(n);

    /* Fill missing entries. */
    for (size_t k = cache_len + 1; k <= (size_t)n; ++k) {
        idx_t idx = (idx_t)k;
        seq_t val;
        if (idx == 1) {
            val = (seq_t)1;
        } else if (idx == 2) {
            val = (seq_t)3;
        } else if (idx == 3) {
            val = (seq_t)2;
        } else {
            /* Compute p = floor(log2(idx-2)) using shift loop. */
            idx_t t = idx - 2;          /* idx-2 >= 1 for idx>=4 */
            idx_t pow2 = 1;
            while ((pow2 << 1) <= t)
                pow2 <<= 1;          /* pow2 = 2^p */
            idx_t two_p1 = pow2 << 1;   /* 2^(p+1) */
            idx_t m = 2 * idx - 3 - two_p1;  /* m < idx by construction */
            val = cache[(size_t)idx - 1] + cache[(size_t)m];
        }
        cache[(size_t)k] = val;
    }
    cache_len = (size_t)n;
    return cache[(size_t)n];
}

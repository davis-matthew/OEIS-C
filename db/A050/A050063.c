/*
 * A050063  a(n)=a(n-1)+a(m), where m=2n-2-2^(p+1) and 2^p<n-1<=2^(p+1), for n
 *          >= 4.
 * https://oeis.org/A050063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(m), m = 2*n - 2 - 2^(p+1), p is such that 2^p
 *            < n-1 <= 2^(p+1), a(1)=1, a(2)=3, a(3)=2. (OEIS sequence
 *            A050063)
 * Range:     u8 n <= 14, u16 n <= 48, u32 n <= 125, u64 n <= 504,
 *            u128 n <= 3081, big any n
 * Validated: standard, pass, 58 terms through n = 6995 (2026-11-03)
 * Helpers:   ilog2, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ilog2.h"
#include "../utils/mem.h"

static seq_t *cache = NULL;   /* 1‑indexed array of computed values */
static idx_t cache_cap = 0;   /* allocated capacity */
static idx_t cache_sz  = 0;   /* highest computed index */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (n <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < n) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, (new_cap + 1));
    if (!tmp) exit(1);     /* simplest error handling */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) for any n >= 1.  Results are memoised. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, return 0 for safety */

    if (n > cache_sz) {
        ensure_capacity(n);
        for (idx_t i = cache_sz + 1; i <= n; ++i) {
            if (i == 1)      cache[i] = 1;
            else if (i == 2) cache[i] = 3;
            else if (i == 3) cache[i] = 2;
            else {
                /* Compute the auxiliary index m. */
                idx_t v     = i - 1;                /* v >= 3 */
                unsigned int p = ilog2(v);
                if (((idx_t)1 << p) == v) p--;      /* exact power of two? */
                idx_t pow2next = ((idx_t)1) << (p + 1);
                idx_t m = (2 * i - 2) - pow2next;   /* m < i and >= 1 */
                cache[i] = cache[i - 1] + cache[m];
            }
        }
        cache_sz = n;
    }
    return cache[n];
}

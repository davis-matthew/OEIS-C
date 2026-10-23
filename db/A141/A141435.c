/*
 * A141435  a(1) = 1, a(2) = 2; a(n) = a(n-a(1)) + a(n-a(2)) + a(n-a(3)) +
 *          a(n-a(4)) + ...
 * https://oeis.org/A141435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=2; for n≥3, a(n) = Σ a(n-a(k)) for all k
   with a(k) < n (i.e., k=1,2,…,kmax where a(kmax) < n ≤ a(kmax+1)). */
#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[0] = a(1), cache[1] = a(2), …   */
static idx_t  max_cached = 0;    /* highest n for which a(n) is known   */

/* Resize cache to hold values up to n (inclusive). */
static void ensure_cache_size(idx_t n)
{
    if (cache == NULL) {
        size_t cap = 16;
        if (cap < (size_t)n) cap = (size_t)n;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
    } else if (max_cached < n) {
        size_t cap = max_cached;
        while (cap < (size_t)n) cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                       /* undefined for n<1 */

    if (n > max_cached) {                   /* extend cache to n */
        ensure_cache_size(n);

        for (idx_t k = max_cached + 1; k <= n; ++k) {
            if (k == 1)           cache[0] = 1;
            else if (k == 2)      cache[1] = 2;
            else {
                seq_t sum = 0;
                for (idx_t j = 1; j < k; ++j) {
                    seq_t ak = cache[j-1];     /* a(j) */
                    if (ak >= (seq_t)k) break; /* a(j) ≥ n → stop */
                    idx_t idx = k - (idx_t)ak;  /* n‑a(j) */
                    sum += cache[idx-1];
                }
                cache[k-1] = sum;
            }
        }
        max_cached = n;
    }

    return cache[n-1];
}

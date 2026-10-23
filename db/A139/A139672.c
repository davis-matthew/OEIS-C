/*
 * A139672  Convolution of A008619 and A001400.
 * https://oeis.org/A139672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   The sequence A139672 has generating function G(x) =
 *            x/((x^2+x+1)*(x^2+1)*(x+1)^3*(x-1)^6). Equivalently it satisfies
 *            the linear recurrence of order 13 (q_k from the denominator
 *            expansion): a(n) + (-2)a(n-1) + (-1)a(n-2) + 3a(n-3) + 0*a(n-4)
 *            + 1a(n-5) + (-2)a(n-6) + (-2)a(n-7) + 1a(n-8) + 0*a(n-9)...
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 471, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Coefficients q_k for the recurrence, q_0 = 1. */
static const int Q[14] = {1, -2, -1, 3, 0, 1, -2, -2, 1, 0, 3, -1, -2, 1};

/* Initial 13 terms, indexed 0..12 correspond to a(1)..a(13). */
static const seq_t INIT[13] = {
    (seq_t)1, (seq_t)2, (seq_t)5, (seq_t)9, (seq_t)17,
    (seq_t)27, (seq_t)44, (seq_t)65, (seq_t)97, (seq_t)136,
    (seq_t)191, (seq_t)257, (seq_t)346
};

/* Dynamic cache: cache[i] holds a(i+1). */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of terms stored */
static idx_t cache_cap = 0;   /* allocated capacity */

/* Resize the cache so that it can hold at least new_cap terms. */
static void ensure_capacity(idx_t new_cap)
{
    if (new_cap <= cache_cap)
        return;

    seq_t *tmp;
#if defined(__cplusplus)
    tmp = new seq_t[new_cap];
    /* Copy existing terms */
    for (idx_t i = 0; i < cache_len; ++i)
        tmp[i] = cache[i];
    delete[] cache;
#else
    tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* realloc preserves existing contents */
    if (cache == NULL)
        cache = tmp;
#endif
    cache = tmp;
    cache_cap = new_cap;
}

/* Fill the cache with values up to (and including) index n (1‑based). */
static void ensure_cache(idx_t n)
{
    if (n == 0 || n <= cache_len)
        return;

    /* Ensure enough storage */
    ensure_capacity(n);

    /* Load initial terms if not already present */
    if (cache_len < 13) {
        idx_t start = cache_len;
        idx_t end = n < 13 ? n : 13;
        for (idx_t i = start; i < end; ++i)
            cache[i] = INIT[i];
        cache_len = end;
    }

    /* Compute further terms via the recurrence */
    for (idx_t i = cache_len; i < n; ++i) {
        /* Compute sum_{k=1}^{13} q_k * a(i+1 - k) */
        seq_t sum = 0;
        for (int k = 1; k <= 13; ++k) {
            sum += (seq_t)Q[k] * cache[i - k];
        }
        /* a(i+1) = -sum, because Q[0] = 1 */
        cache[i] = -sum;
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0 */
    ensure_cache(n);
    return cache[n - 1];
}

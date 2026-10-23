/*
 * A152537  Convolution sequence: this sequence convolved with A000041 gives
 *          powers of 2, (A000079).
 * https://oeis.org/A152537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * a(n-1) + c(n), a(0) = 1; * where c(n) are the
 *            coefficients of ∏_{k≥1}(1‑x^k) * (Euler's pentagonal numbers:
 *            c(0)=1, c(n)=(-1)^k if n=k(3k±1)/2, else 0)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static idx_t last_n = (idx_t)-1;

/* return coefficient c(n) of (1-x)(1-x²)(1-x³)… */
static int8_t coeff_c(idx_t n)
{
    if (n == 0) return 1;
    int8_t sign = 0;
    for (idx_t k = 1;; ++k) {
        idx_t p = k * (3 * k - 1) / 2;
        if (p > n) break;
        if (p == n) { sign = (k & 1) ? -1 : 1; break; }
    }
    if (sign == 0) {
        for (idx_t k = 1;; ++k) {
            idx_t p = k * (3 * k + 1) / 2;
            if (p > n) break;
            if (p == n) { sign = (k & 1) ? -1 : 1; break; }
        }
    }
    return sign;
}

seq_t a(idx_t n)
{
    if (cache == NULL) {
        cache_len = 1;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;       /* a(0) */
        last_n   = 0;
    }

    if (n > last_n) {
        size_t new_len = (size_t)n + 1;
        if (new_len > cache_len) {
            seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_len = new_len;
        }
        for (idx_t i = last_n + 1; i <= n; ++i) {
            seq_t val = cache[i-1] + cache[i-1];   /* 2 * a(i-1) */
            int8_t c = coeff_c(i);
            if (c > 0) val = val + (seq_t)1;
            else if (c < 0) val = val - (seq_t)1;
            cache[i] = val;
        }
        last_n = n;
    }
    return cache[n];
}

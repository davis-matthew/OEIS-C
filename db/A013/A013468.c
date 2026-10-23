/*
 * A013468  Expansion of e.g.f.:
 *          exp(cos(x)-exp(x))=1-x-1/2!*x^2+4/3!*x^3+5/4!*x^4-32/5!*x^5...
 * https://oeis.org/A013468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; for n≥1, a(n)=∑_{k=1}^{n} binom(n-1,k-1)·c(k)·a(n-k)
 *            where c(k)=0 if k%4==0, -1 if k odd, -2 if k≡2 (mod 4)
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 16, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   c_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/c_val.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (cache_len == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        for (idx_t i = cache_len; i < new_len; ++i) {
            seq_t sum = (seq_t)0;
            seq_t C = (seq_t)1;              /* binom(i-1,0) */
            for (idx_t k = 1; k <= i; ++k) {
                if (k > 1) {
                    C = C * (i - k + 1) / (k - 1);
                }
                int ck = c_val(k);
                if (ck != 0) {
                    seq_t term = C * (seq_t)ck * cache[i - k];
                    sum += term;
                }
            }
            cache[i] = sum;
        }
        cache_len = new_len;
    }
    return cache[n];
}

/*
 * A219673  a(n) = Sum_{k=0..n} binomial(n,k)^2*Lucas(k) where Lucas(n) =
 *          A000032(n).
 * https://oeis.org/A219673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^2 * Lucas(k) (Lucas(0)=2,
 *            Lucas(1)=1, Lucas(k)=Lucas(k-1)+Lucas(k-2)) Implementation uses
 *            a direct summation with incremental binomial calculation. Cache
 *            is used for Lucas numbers to avoid recomputing. The function
 *            memoizes results, so future calls for larger n a...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t* lucas_cache = NULL;
static size_t lucas_cap = 0;

static void ensure_lucas(idx_t n) {
    if (lucas_cap > n) return;
    size_t new_cap = n + 1;
    lucas_cache = (seq_t*)realloc(lucas_cache, new_cap * sizeof(seq_t));
    if (!lucas_cache) exit(EXIT_FAILURE);  /* allocation failure */
    for (size_t i = lucas_cap; i <= n; ++i) {
        if (i == 0) lucas_cache[i] = (seq_t)2;
        else if (i == 1) lucas_cache[i] = (seq_t)1;
        else lucas_cache[i] = lucas_cache[i-1] + lucas_cache[i-2];
    }
    lucas_cap = new_cap;
}

seq_t a(idx_t n) {
    ensure_lucas(n);
    seq_t sum = 0;
    seq_t b = 1;     /* binomial(n,0) */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t bsq = b * b;
        sum += bsq * lucas_cache[k];
        if (k < n) {
            /* b = binomial(n,k+1) = binomial(n,k)*(n-k)/(k+1) */
            seq_t num = b * (seq_t)(n - k);
            b = num / (seq_t)(k + 1);
        }
    }
    return sum;
}

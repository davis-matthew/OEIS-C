/*
 * A171792  G.f. A(x) satisfies: A(x) = (x + A(x+x^2))/2 with A(0)=0.
 * https://oeis.org/A171792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 a(n) = Σ_{k=1}^{⌊n/2⌋} a(n‑k) · C(n‑k, k) (n ≥ 2) where
 *            C(x, y) is the binomial coefficient (x choose y)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Local implementation of C(n, k) using a multiplicative loop.
   It works for all supported seq_t types (built‑in integers or
   big‑integer classes) because it only uses the native +, -, *, /, % operators. */
static inline seq_t binom_local(idx_t n, idx_t k)
{
    if (k > n)          return 0;
    if (k > n - k)      k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = (res * (n - k + i)) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* out of range, harmless */

    static seq_t *cache = NULL;     /* memoised values of a(m) */
    static size_t cap   = 0;        /* array capacity */
    static idx_t  max_comp = 0;     /* highest index computed */

    /* initialise cache on first call */
    if (!cache) {
        cap = 4;                    /* room for indices 0..3 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed */
        cache[0] = 0;
        cache[1] = 1;
        max_comp = 1;
    }

    /* grow cache if necessary */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while ((size_t)n >= newcap) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        for (size_t i = cap; i < newcap; ++i) tmp[i] = 0;
        cache = tmp;
        cap = newcap;
    }

    /* compute missing terms up to n */
    if (n > max_comp) {
        for (idx_t m = max_comp + 1; m <= n; ++m) {
            idx_t limit = m / 2;       /* floor(m/2) */
            seq_t sum   = 0;
            if (limit >= 1) {
                /* k = 1: C(m-1,1) = m-1 */
                seq_t bin = (seq_t)(m - 1);
                sum += bin * cache[m - 1];
                for (idx_t k = 2; k <= limit; ++k) {
                    seq_t num1  = (seq_t)(m - 2 * k + 2);
                    seq_t num2  = (seq_t)(m - 2 * k + 1);
                    seq_t denom = (seq_t)k * (m - k + 1);
                    bin = (bin * num1 * num2) / denom;
                    idx_t r = m - k;     /* corresponding a(r) */
                    sum += bin * cache[r];
                }
            }
            cache[m] = sum;
        }
        max_comp = n;
    }

    return cache[n];
}

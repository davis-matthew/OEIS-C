/*
 * A137697  a(n) = x(n) * 2^((n mod 2 - 1)/2), with
 *          x(n)=Sum(x(k)*x(n-k-1):0<=k<n), x(0)=SQRT(2).
 * https://oeis.org/A137697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 2; For n >= 2: if n is even: a(n) =
 *            Σ_{k=0}^{n-1} a(k) * a(n-1-k) if n is odd : a(n) = Σ_{k=0}^{n-1}
 *            ((k%2==1)?1:2) * a(k) * a(n-1-k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    if (cache.empty()) { cache.resize(1); cache[0] = seq_t(1); }
    size_t old_size = cache.size();
    if ((size_t)n >= old_size) {
        cache.resize((size_t)n + 1);
        for (idx_t i = (idx_t)old_size; i <= n; ++i) {
            seq_t sum = 0;
            idx_t im1 = i - 1;
            if ((i & 1) == 0) {            /* n even */
                for (idx_t k = 0; k < i; ++k)
                    sum = sum + cache[k] * cache[im1 - k];
            } else {                       /* n odd */
                for (idx_t k = 0; k < i; ++k) {
                    seq_t prod = cache[k] * cache[im1 - k];
                    if ((k & 1) == 0) prod = prod * seq_t(2);
                    sum = sum + prod;
                }
            }
            cache[i] = sum;
        }
    }
    return cache[(size_t)n];
#else
    static seq_t *cache = NULL;
    static idx_t last_idx = (idx_t)-1;

    if (last_idx == (idx_t)-1) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = 0;             /* placeholder for compile */
        cache[0] = (seq_t)1;
        last_idx = 0;
    }

    if (n > last_idx) {
        size_t new_cap = (size_t)last_idx + 1;
        if (new_cap < 2 * (size_t)(last_idx + 1)) new_cap = 2 * (size_t)(last_idx + 1);
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failed */
        cache = tmp;
    }

    for (idx_t i = last_idx + 1; i <= n; ++i) {
        seq_t sum = 0;
        idx_t im1 = i - 1;
        if ((i & 1) == 0) {             /* n even */
            for (idx_t k = 0; k < i; ++k)
                sum = sum + cache[k] * cache[im1 - k];
        } else {                        /* n odd */
            for (idx_t k = 0; k < i; ++k) {
                seq_t prod = cache[k] * cache[im1 - k];
                if ((k & 1) == 0) prod = prod * (seq_t)2;
                sum = sum + prod;
            }
        }
        cache[i] = sum;
    }
    last_idx = n;
    return cache[n];
#endif
}

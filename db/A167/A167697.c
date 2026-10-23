/*
 * A167697  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*(a(n-1)+a(n-2)+…+a(n-14)) – 300*a(n-15) for n≥15, with
 *            initial terms a(0)=1, a(1)=26, a(2)=650, …,
 *            a(15)=968575477600097655925
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__cplusplus)
#include <vector>
#else
#include <stddef.h>
#include <stdlib.h>
#endif

seq_t a(idx_t n)
{
#if defined(__cplusplus)
    /* C++ implementation using std::vector to handle arbitrary‑precision seq_t */
    static std::vector<seq_t> cache;
    if (cache.empty())
        cache.push_back(seq_t(1));               /* a(0) */

    if (cache.size() <= n) {
        size_t old_size = cache.size();
        cache.resize((size_t)n + 1);
        static const seq_t f[16] = {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1};

        for (size_t i = old_size; i <= (size_t)n; ++i) {
            seq_t sum = 0;
            for (size_t j = 1; j <= 14 && i >= j; ++j)
                sum += cache[i - j];
            sum *= (seq_t)24;
            if (i >= 15)
                sum -= cache[i - 15] * (seq_t)300;
            if (i <= 15)
                sum += f[i];
            cache[i] = sum;
        }
    }

    return cache[(size_t)n];
#else
    /* C implementation using realloc on a raw array */
    static seq_t *cache = NULL;
    static size_t cap    = 0;

    if (cap <= n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;                      /* assume allocation succeeds */

        static const seq_t f[16] = {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1};

        for (idx_t i = (idx_t)cap; i <= n; ++i) {
            seq_t sum = 0;
            for (int j = 1; j <= 14 && i >= (idx_t)j; ++j)
                sum += cache[i - j];
            sum *= (seq_t)24;
            if (i >= (idx_t)15)
                sum -= cache[i - 15] * (seq_t)300;
            if (i <= (idx_t)15)
                sum += f[(size_t)i];
            cache[i] = sum;
        }
        cap = new_cap;
    }

    return cache[(size_t)n];
#endif
}

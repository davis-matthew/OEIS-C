/*
 * A104455  Expansion of e.g.f. exp(5*x)*(BesselI(0,2*x) - BesselI(1,2*x)).
 * https://oeis.org/A104455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A104455
    Formula (recurrence): a(0) = 1
                          a(n) = 3^n + Σ_{k=0}^{n-1} a(k)*a(n-1-k)
    Memoized incremental computation, O(n²) additions/multiplications
*/

#ifdef __cplusplus
#include <vector>
#include <cstdlib>
#else
#include <stdlib.h>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation */
    static std::vector<seq_t> cache;   /* already computed terms */
    static seq_t pow3 = (seq_t)1;     /* 3^current index (index of next term) */

    if (cache.empty()) {
        cache.reserve(16);
        cache.push_back((seq_t)1);    /* a(0) = 1 */
        pow3 = (seq_t)1;
    }

    /* grow cache up to n */
    while (static_cast<idx_t>(cache.size()) <= n) {
        const idx_t i = static_cast<idx_t>(cache.size());
        pow3 = pow3 * (seq_t)3;      /* 3^i */

        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < i; ++k)
            sum += cache[k] * cache[i - 1 - k];

        cache.push_back(pow3 + sum);
    }

    return cache[n];

#else
    /* C implementation */
    static seq_t *cache = NULL;  /* dynamic array of terms */
    static idx_t  cap   = 0;     /* allocated size */
    static idx_t  count = 0;     /* next index to calculate (size of cache) */
    static seq_t pow3   = (seq_t)0;   /* 3^current index */

    if (count == 0) {           /* first call */
        cap   = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;    /* a(0) = 1 */
        count   = 1;            /* next index to compute */
        pow3    = (seq_t)1;
    }

    while (count <= n) {
        if (count >= cap) {     /* grow array */
            cap <<= 1;
            void *tmp = realloc(cache, cap * sizeof(seq_t));
            if (!tmp) { free(cache); exit(1); }
            cache = (seq_t *)tmp;
        }

        const idx_t i = count;
        pow3 = pow3 * (seq_t)3;          /* 3^i */

        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < i; ++k)
            sum += cache[k] * cache[(size_t)(i - 1 - k)];

        cache[i] = pow3 + sum;
        ++count;
    }

    return cache[n];
#endif
}

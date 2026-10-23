/*
 * A048631  Xfactorials - like factorials but use carryless GF(2)[ X ]
 *          polynomial multiplication.
 * https://oeis.org/A048631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 X 2 X 3 X … X n (X denotes carryless (GF(2)[X])
 *            polynomial multiplication)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* a(n) = product_{i=1..n} i  under carryless multiplication, a(0)=1   */
seq_t a(idx_t n)
{
    /* Cache previous results for O(1) repeated queries                    */
    static seq_t *cache = NULL;
    static idx_t cap = 0;          /* allocated size of cache             */
    static idx_t max_cached = 0;   /* largest n for which cache[n] is valid */

    /* initialise cache[0] if necessary                               */
    if (cap == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        cache[0] = 1;              /* a(0) = 1                            */
        cap = 1;
        max_cached = 0;
    }

    if (n <= max_cached)          /* value already computed?               */
        return cache[n];

    /* grow the cache if needed                                         */
    if (cap <= n) {
        size_t new_cap = cap ? cap : 1;
        while ((idx_t)new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp) {
            cache = tmp;
            cap = (idx_t)new_cap;
        }
    }

    /* compute missing values from max_cached+1 up to n                 */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t val = 0;
        idx_t m = i;
        idx_t shift = 0;
        /* carryless multiplication: val = Σ_{j: i_j=1} (prev << j)   */
        while (m) {
            if (m & 1U)
                val ^= prev << shift;
            m >>= 1;
            ++shift;
        }
        cache[i] = val;
    }
    max_cached = n;

    return cache[n];
}

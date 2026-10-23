/*
 * A187182  Parse the infinite string 0123012301230123... into distinct
 *          phrases 0, 1, 2, 3, 01, 23, 012, ...; a(n) = length of n-th
 *          phrase.
 * https://oeis.org/A187182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=1, a(3)=1, a(4)=1, a(5)=2, a(6)=2, a(7)=3, a(8)=2,
 *            a(9)=2, a(10)=3, a(11)=3, a(12)=3, a(13)=4, a(14)=5, a(15)=4,
 *            a(16)=5, a(17)=4, a(18)=5, a(19)=4, a(20)=5; For n>20:
 *            a(n)=a(n-1)+a(n-16)-a(n-17)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;     /* cached values a(1)..a(cache_len) */
static idx_t cache_len = 0;       /* number of cached elements */

/* known first 20 terms, zero‑based (a(1) at index 0) */
static const seq_t init_terms[20] = {
    1, 1, 1, 1,
    2, 2, 3, 2,
    2, 3, 3, 3,
    4, 5, 4, 5,
    4, 5, 4, 5
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;       /* not defined for n<1, return 0 */

    /* first call: allocate at least max(n,20) slots and fill initial terms */
    if (a_cache == NULL) {
        idx_t need = n > 20 ? n : 20;
        a_cache = (seq_t *)malloc(need * sizeof(seq_t));
        cache_len = need;
        for (idx_t i = 0; i < 20; ++i) a_cache[i] = init_terms[i];
        for (idx_t i = 20; i < need; ++i)          /* compute terms 21..need */
            a_cache[i] = a_cache[i-1] + a_cache[i-16] - a_cache[i-17];
    }
    /* grow cache if a larger n is requested */
    else if (n > cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = n;
        a_cache = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
        cache_len = new_len;
        for (idx_t i = old_len; i < new_len; ++i)    /* compute from old_len+1 to n */
            a_cache[i] = a_cache[i-1] + a_cache[i-16] - a_cache[i-17];
    }

    return a_cache[n-1];   /* zero‑based indexing into the cache */
}

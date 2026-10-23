/*
 * A166414  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^11 = I.
 * https://oeis.org/A166414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*Sum_{j=1..10} a(n-j) - 171*a(n-11), with a(0)=1,
 *            a(1)=20, a(2)=380, a(3)=7220, a(4)=137180, a(5)=2606420,
 *            a(6)=49521980, a(7)=940917620, a(8)=17877434780,
 *            a(9)=339671260820, a(10)=6453753955580. For n=11 the recurrence
 *            gives a(11)=18*sum_{j=1..10} a(j)-171*a(0)+1 (i.e. subtract 170
 *            inst...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL; /* cache of computed terms */
    static size_t   len   = 0;  /* number of terms stored */
    static size_t   cap   = 0;  /* capacity of cache buffer */

    /* convert the index to a non-unsigned size_t for array access */
    size_t i = (size_t)n;

    /* grow cache if necessary */
    if (i + 1 > cap) {
        size_t newcap = cap ? cap * 2 : 16;
        if (newcap < i + 1) newcap = i + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = newcap;
    }

    /* compute terms up to i if not already cached */
    for (size_t j = len; j <= i; ++j) {
        if (j < 11) { /* base values */
            static const seq_t base[11] = {
                1ULL,         20ULL,         380ULL,         7220ULL,
                137180ULL,    2606420ULL,    49521980ULL,    940917620ULL,
                17877434780ULL,339671260820ULL,6453753955580ULL
            };
            cache[j] = base[j];
        } else { /* recurrence for j >= 11 */
            seq_t sum = 0;
            for (size_t k = 1; k <= 10; ++k)
                sum += cache[j - k];
            seq_t term = 18 * sum - 171 * cache[j - 11];
            if (j == 11) /* special adjustment for a(11) */
                term += 1;
            cache[j] = term;
        }
    }

    len = i + 1;
    return cache[i];
}

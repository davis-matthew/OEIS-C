/*
 * A219817  Number of 3 X n arrays of the minimum value of corresponding
 *          elements and their horizontal, vertical or diagonal neighbors in a
 *          random, but sorted with lexicographically nondecreasing rows and
 *          nonincreasing columns, 0..2 3 X n array.
 * https://oeis.org/A219817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 45*a(n-2) + 120*a(n-3) - 210*a(n-4) +
 *            252*a(n-5) - 210*a(n-6) + 120*a(n-7) - 45*a(n-8) + 10*a(n-9) -
 *            a(n-10), for n > 13. Initial values: a(1)= 6, a(2)= 16, a(3)=
 *            62, a(4)=165, a(5)=377, a(6)=795, a(7)=1588, a(8)=3039,
 *            a(9)=5612, a(10)=10061, a(11)=17603, a(12)=30184, a(13)=50875
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 49, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base[14] = {
    0ULL,       /* unused index 0 */
    6ULL, 16ULL, 62ULL, 165ULL, 377ULL, 795ULL,
    1588ULL, 3039ULL, 5612ULL, 10061ULL, 17603ULL,
    30184ULL, 50875ULL
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* make the function safe for n = 0 */

    static seq_t *cache = NULL;     /* cache[1 .. cap] holds computed values */
    static idx_t cap  = 0;          /* highest index currently cached */

    if (n <= cap)
        return cache[n];

    /* grow cache to accommodate index n */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) return 0;       /* allocation failure – unlikely in tests */
    cache = new_cache;

    /* initialise first terms that are not yet cached */
    for (idx_t i = cap + 1; i <= n && i <= 13; ++i)
        cache[i] = base[i];

    /* compute terms from max(cap,13)+1 up to n */
    idx_t start = (cap > 13) ? cap : 13;
    for (idx_t i = start + 1; i <= n; ++i)
    {
        seq_t pos = cache[i-1] * 10
                  + cache[i-3] * 120
                  + cache[i-5] * 252
                  + cache[i-7] * 120
                  + cache[i-9] * 10;

        seq_t neg = cache[i-2] * 45
                  + cache[i-4] * 210
                  + cache[i-6] * 210
                  + cache[i-8] * 45
                  + cache[i-10];

        cache[i] = pos - neg;
    }

    cap = n;
    return cache[n];
}

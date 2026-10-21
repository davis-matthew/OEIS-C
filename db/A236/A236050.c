/*
 * A236050  Number of (n+1)X(3+1) 0..2 arrays colored with the difference of
 *          the maximum and minimum in each 2X2 subblock.
 * https://oeis.org/A236050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-2) + 42*a(n-4) - 21*a(n-6) - 207*a(n-8) for n > 13
 *            Initial values (for the recurrence) are: a(1)=1112, a(2)=3516,
 *            a(3)=12686, a(4)=32608, a(5)=103986, a(6)=297316, a(7)=981554,
 *            a(8)=2710176, a(9)=8760338, a(10)=24641412, a(11)=80155474,
 *            a(12)=224030592, a(13)=726570374, a(14)=2036634156
 * Range:     u8 none, u16 n <= 4, u32 n <= 14, u64 n <= 34, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[i] holds a(i) for i >= 0 */
static idx_t cached_len = 0;     /* number of initialized entries */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0;   /* a(0) is undefined; return 0 for safety */

    /* Ensure the cache is large enough */
    if (cached_len <= n) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;              /* Allocation failed */
        cache = tmp;

        /* Fill newly allocated entries */
        for (size_t i = (size_t)cached_len; i <= n; ++i) {
            if (i == (size_t)1)          cache[i] = (seq_t)1112ULL;
            else if (i == (size_t)2)     cache[i] = (seq_t)3516ULL;
            else if (i == (size_t)3)     cache[i] = (seq_t)12686ULL;
            else if (i == (size_t)4)     cache[i] = (seq_t)32608ULL;
            else if (i == (size_t)5)     cache[i] = (seq_t)103986ULL;
            else if (i == (size_t)6)     cache[i] = (seq_t)297316ULL;
            else if (i == (size_t)7)     cache[i] = (seq_t)981554ULL;
            else if (i == (size_t)8)     cache[i] = (seq_t)2710176ULL;
            else if (i == (size_t)9)     cache[i] = (seq_t)8760338ULL;
            else if (i == (size_t)10)    cache[i] = (seq_t)24641412ULL;
            else if (i == (size_t)11)    cache[i] = (seq_t)80155474ULL;
            else if (i == (size_t)12)    cache[i] = (seq_t)224030592ULL;
            else if (i == (size_t)13)    cache[i] = (seq_t)726570374ULL;
            else if (i == (size_t)14)    cache[i] = (seq_t)2036634156ULL;
            else { /* recurrence for i > 14 */
                cache[i] = ((seq_t)5)     * cache[i-2]
                         + ((seq_t)42)    * cache[i-4]
                         - ((seq_t)21)    * cache[i-6]
                         - ((seq_t)207)   * cache[i-8];
            }
        }
        cached_len = new_len;
    }
    return cache[(size_t)n];
}

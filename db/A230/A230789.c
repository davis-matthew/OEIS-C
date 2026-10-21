/*
 * A230789  Number of (n+3)X(1+3) 0..3 arrays x(i,j) with each element
 *          diagonally or antidiagonally next to at least one element with
 *          value (x(i,j)+1) mod 4, and upper left element zero.
 * https://oeis.org/A230789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) + 43*a(n-2) + 767*a(n-3) + 2043*a(n-4)
 *            -9055*a(n-5) -43349*a(n-6) +219*a(n-7) +23245*a(n-8)
 *            -6325*a(n-9) +18361*a(n-10) -763*a(n-11) +2745*a(n-12)
 *            -213*a(n-13) -15*a(n-14) + a(n-15); Initial values: a(1)=144,
 *            a(2)=1024, a(3)=23104, a(4)=394384, a(5)=5475600, a(6)=84052224,
 *            a(7)=13...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Constant initial values (a[1]..a[15]) */
    static const seq_t init[15] = {
        144ULL,
        1024ULL,
        23104ULL,
        394384ULL,
        5475600ULL,
        84052224ULL,
        1326125056ULL,
        20301690256ULL,
        310824090256ULL,
        4792596640000ULL,
        73781640461376ULL,
        1134353447066896ULL,
        17451552419496976ULL,
        268530609869169664ULL,
        4131141192676642816ULL
    };

    static seq_t *cache = NULL;          /* 1‑based cache */
    static size_t cache_len = 0;          /* highest index computed */
    static size_t cache_cap = 0;          /* allocated capacity (size_t) */

    if (n == 0)  /* safe guard, should not happen for valid queries */
        return 0;

    /* Allocate or expand cache as needed */
    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) /* Allocation failed – return 0 as fallback */
            return 0;
        /* Zero any newly allocated space – not strictly needed */
        if (new_cap > cache_cap)
            memset(tmp + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n, starting from next needed index */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        if (i <= 15) {
            cache[i] = init[i - 1];
        } else {
            seq_t val = 0;
            val += ((seq_t)9)   * cache[i - 1];
            val += ((seq_t)43)  * cache[i - 2];
            val += ((seq_t)767) * cache[i - 3];
            val += ((seq_t)2043) * cache[i - 4];
            val -= ((seq_t)9055) * cache[i - 5];
            val -= ((seq_t)43349) * cache[i - 6];
            val += ((seq_t)219)  * cache[i - 7];
            val += ((seq_t)23245) * cache[i - 8];
            val -= ((seq_t)6325) * cache[i - 9];
            val += ((seq_t)18361) * cache[i - 10];
            val -= ((seq_t)763)  * cache[i - 11];
            val += ((seq_t)2745) * cache[i - 12];
            val -= ((seq_t)213)  * cache[i - 13];
            val -= ((seq_t)15)   * cache[i - 14];
            val += cache[i - 15];
            cache[i] = val;
        }
    }
    cache_len = (size_t)n;
    return cache[n];
}

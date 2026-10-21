/*
 * A229752  Number of defective 4-colorings of an n X 5 0..3 array connected
 *          horizontally, vertically, diagonally and antidiagonally with
 *          exactly two mistakes, and colors introduced in row-major 0..3
 *          order.
 * https://oeis.org/A229752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 33*a(n-2) + 63*a(n-3) - 66*a(n-4) * +
 *            36*a(n-5) - 8*a(n-6) (for n > 11) with initial terms: * a(1)=12,
 *            a(2)=1840, a(3)=6809, a(4)=15075, a(5)=29776, * a(6)=57677,
 *            a(7)=113330, a(8)=228657, a(9)=473562, * a(10)=1000381,
 *            a(11)=2139866
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 20, u64 n <= 51, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already-computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* current allocated size of cache   */
    static idx_t known  = 0;      /* largest n for which cache[n] is valid */

    if (n == 0) return 0;         /* undefined but harmless */

    /* grow cache if necessary to hold index n */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        if (new_cap < (size_t)(n + 1))
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cap   = new_cap;
    }

    /* initial (non-recursive) values */
    static const seq_t seed[11] = {
        12ULL, 1840ULL, 6809ULL, 15075ULL, 29776ULL,
        57677ULL, 113330ULL, 228657ULL, 473562ULL,
        1000381ULL, 2139866ULL
    };

    /* fill missing entries up to n */
    for (idx_t i = known + 1; i <= n; ++i) {
        if (i <= 11) {
            cache[i] = seed[i - 1];
        } else {
            seq_t pos = 9 * cache[i - 1];
            pos += 63 * cache[i - 3];
            pos += 36 * cache[i - 5];

            seq_t neg = 33 * cache[i - 2];
            neg += 66 * cache[i - 4];
            neg += 8 * cache[i - 6];

            cache[i] = pos - neg;
        }
    }
    known = n;
    return cache[n];
}

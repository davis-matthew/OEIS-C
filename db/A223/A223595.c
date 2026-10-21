/*
 * A223595  Petersen graph (8,2) coloring a rectangular array: number of nX4
 *          0..15 arrays where 0..15 label nodes of a graph with edges 0,1 0,8
 *          8,14 8,10 1,2 1,9 9,15 9,11 2,3 2,10 10,12 3,4 3,11 11,13 4,5 4,12
 *          12,14 5,6 5,13 13,15 6,7 6,14 7,0 7,15 and every array movement to
 *          a horizontal, diagonal or antid...
 * https://oeis.org/A223595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 45*a(n-1) -639*a(n-2) +2781*a(n-3) +4328*a(n-4) *
 *            -42674*a(n-5) +32672*a(n-6) +131496*a(n-7) -190080*a(n-8)
 *            +31104*a(n-9) * Initial values for 1 ≤ n ≤ 9: * a(1)=432,
 *            a(2)=6736, a(3)=122608, a(4)=2372080, * a(5)=47659632,
 *            a(6)=982848688, a(7)=20631729648, * a(8)=438231627440,
 *            a(9)=937990592...
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Fixed initial terms for n = 1 .. 9 */
    static const seq_t init[9] = {
        (seq_t)432ULL,
        (seq_t)6736ULL,
        (seq_t)122608ULL,
        (seq_t)2372080ULL,
        (seq_t)47659632ULL,
        (seq_t)982848688ULL,
        (seq_t)20631729648ULL,
        (seq_t)438231627440ULL,
        (seq_t)9379905920496ULL
    };

    /* Recurrence coefficients (for a(1)..a(9)) */
    static const int coeff[9] = {
        45,     -639,    2781,    4328,
        -42674, 32672,   131496,  -190080,
        31104
    };

    /* Cache already-computed terms (0‑based: cache[0] = a(1)) */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static size_t  len    = 0;   /* number of terms stored */

    /* Return known values immediately */
    if (n <= 9ULL)
        return init[(size_t)n - 1];

    /* Ensure the cache can accommodate n terms */
    if (cap < (size_t)n) {
        size_t newcap = cap ? cap * 2 : 16;
        if (newcap < (size_t)n)
            newcap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();   /* out of memory */
        cache = tmp;
        cap   = newcap;
    }

    /* Initialise cache on first use */
    if (len == 0) {
        for (size_t i = 0; i < 9; ++i)
            cache[i] = init[i];
        len = 9;
    }

    /* Compute missing terms in numerical order */
    while (len < (size_t)n) {
        size_t idx = len;   /* idx corresponds to a(idx+1) */

        seq_t pos = 0;   /* sum of positive contributions */
        seq_t neg = 0;   /* sum of absolute values of negative contributions */

        for (int j = 0; j < 9; ++j) {
            seq_t term = cache[idx - 1 - j];
            int     c   = coeff[j];

            if (c >= 0) {
                pos += term * (seq_t)c;
            } else {
                neg += term * (seq_t)(-c);
            }
        }

        cache[idx] = pos - neg;
        ++len;
    }

    return cache[n - 1];
}

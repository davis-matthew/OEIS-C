/*
 * A220631  Number of ways to reciprocally link elements of an nX7 array
 *          either to themselves or to exactly two horizontal or antidiagonal
 *          neighbors.
 * https://oeis.org/A220631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A220631
 * Formula (linear recurrence with 14‑term order):
 *   a(n) = 11*a(n-1) +119*a(n-2) +510*a(n-3) +1074*a(n-4) +735*a(n-5)
 *           -1120*a(n-6) -2716*a(n-7) +5687*a(n-8) -4250*a(n-9)
 *           +1973*a(n-10) -802*a(n-11) +244*a(n-12) -36*a(n-13) -4*a(n-14)
 * Initial values:
 *   a(1)=1, a(2)=32, a(3)=564, a(4)=10695, a(5)=201903, a(6)=3817452,
 *   a(7)=72097810, a(8)=1362188752, a(9)=25734606523,
 *   a(10)=486186026515, a(11)=9185167500820,
 *   a(12)=173528846174065, a(13)=3278357276282501,
 *   a(14)=61935676290875214
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* sequence is 1‑based */

    /* static memoisation cache */
    static seq_t *cache = NULL;
    static size_t cap   = 0;        /* number of elements allocated (index 0 unused) */
    static idx_t  last  = 0;        /* largest index already computed */

    /* order of the recurrence */
    enum { ORDER = 14 };

    /* coefficients of the recurrence (first coefficient is for a(n-1) etc.) */
    static const int64_t coeff[ORDER] = {
        11LL, 119LL, 510LL, 1074LL, 735LL,
        -1120LL, -2716LL, 5687LL, -4250LL, 1973LL,
        -802LL, 244LL, -36LL, -4LL
    };

    /* initial values */
    static const uint64_t init[ORDER] = {
        1ULL, 32ULL, 564ULL, 10695ULL, 201903ULL,
        3817452ULL, 72097810ULL, 1362188752ULL, 25734606523ULL, 486186026515ULL,
        9185167500820ULL, 173528846174065ULL,
        3278357276282501ULL, 61935676290875214ULL
    };

    /* trivial cases: directly return the known initial values */
    if (n <= ORDER) return (seq_t)init[n-1];

    /* grow the cache if necessary */
    if (!cache || cap <= n) {
        size_t new_cap = n + 1;                 /* allocate index 0 .. n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                     /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise the initial segment if it hasn't been done yet */
    if (last < ORDER) {
        for (size_t i = 1; i <= ORDER; ++i) {
            cache[i] = (seq_t)init[i-1];
        }
        last = ORDER;
    }

    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (int k = 0; k < ORDER; ++k) {
            /* multiply by the absolute value of the coefficient */
            seq_t term = cache[i - (k + 1)] * (seq_t)(llabs(coeff[k]));
            if (coeff[k] < 0)
                sum -= term;   /* subtraction for negative coefficient */
            else
                sum += term;   /* addition for positive coefficient */
        }
        cache[i] = sum;
    }
    last = n;
    return cache[n];
}

/*
 * A094429  Given the 3 X 3 matrix M = [0 1 0 / 0 0 1 / 7 -14 7], a(n) = (-)
 *          rightmost term of M^n * [1 1 1].
 * https://oeis.org/A094429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 7*a(n-1) - 14*a(n-2) + 7*a(n-3)
 * Initial values: a(1) = 0, a(2) = 7, a(3) = 42
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* behaviour for n < 1 is irrelevant */

    /* static cache for memoised terms (index 0 unused) */
    static seq_t *cache  = NULL;
    static size_t  cap   = 0;   /* allocated capacity */
    static size_t  upto  = 0;   /* highest index computed so far */

    /* ensure cache can hold up to n (inclusive) */
    size_t req = (size_t)n + 1;
    if (cap < req) {
        size_t newcap = cap ? cap * 2 : 16;
        if (newcap < req) newcap = req;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure – return 0 */
        cache = tmp;
        cap   = newcap;
        /* new slots are uninitialised until written */
    }

    /* initialise base values lazily */
    if (upto == 0) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)42;
        upto = 3;
    }

    /* compute missing values up to n */
    if ((size_t)n > upto) {
        for (size_t i = upto + 1; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)7 * cache[i-1]
                     - (seq_t)14 * cache[i-2]
                     + (seq_t)7 * cache[i-3];
        }
        upto = (size_t)n;
    }

    return cache[(size_t)n];
}

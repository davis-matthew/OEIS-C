/*
 * A220630  Number of ways to reciprocally link elements of an nX6 array
 *          either to themselves or to exactly two horizontal or antidiagonal
 *          neighbors.
 * https://oeis.org/A220630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1)+31*a(n-2)+57*a(n-3)+43*a(n-4)
 *            -3*a(n-5)-36*a(n-6)+19*a(n-7)-6*a(n-8)+a(n-9) Initial values:
 *            a(1)=1, a(2)=16, a(3)=158, a(4)=1665, a(5)=17527, a(6)=183961,
 *            a(7)=1932695, a(8)=20301254, a(9)=213251377
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of the computed terms */
    static seq_t *v = NULL;
    static size_t len = 0;   /* number of initialized terms */
    static size_t cap = 0;   /* allocated capacity */

    if (n == 0) return 0;          /* undefined below offset */

    /* initialise the cache with the first nine values only once */
    if (v == NULL) {
        cap = 9;
#ifdef __cplusplus
        v = new seq_t[cap];
#else
        v = (seq_t *)malloc(cap * sizeof(seq_t));
#endif
        if (!v) return 0;          /* allocation failure */

        v[0] = (seq_t)1;
        v[1] = (seq_t)16;
        v[2] = (seq_t)158;
        v[3] = (seq_t)1665;
        v[4] = (seq_t)17527;
        v[5] = (seq_t)183961;
        v[6] = (seq_t)1932695;
        v[7] = (seq_t)20301254;
        v[8] = (seq_t)213251377;
        len = 9;
    }

    /* resize if the requested index exceeds the current capacity */
    if ((size_t)n > cap) {
        size_t newcap = (size_t)n;
        if (newcap < cap * 2) newcap = cap * 2;
#ifdef __cplusplus
        seq_t *tmp = new seq_t[newcap];
        for (size_t i = 0; i < len; ++i) tmp[i] = v[i];
        delete[] v;
        v = tmp;
#else
        seq_t *tmp = (seq_t *)realloc(v, newcap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */
        v = tmp;
#endif
        cap = newcap;
    }

    /* compute any missing terms up to a(n) */
    if ((size_t)n > len) {
        for (size_t pos = len; pos < (size_t)n; ++pos) {
            seq_t term = 0;
            term += (seq_t)7  * v[pos - 1];
            term += (seq_t)31 * v[pos - 2];
            term += (seq_t)57 * v[pos - 3];
            term += (seq_t)43 * v[pos - 4];
            term -= (seq_t)3  * v[pos - 5];
            term -= (seq_t)36 * v[pos - 6];
            term += (seq_t)19 * v[pos - 7];
            term -= (seq_t)6  * v[pos - 8];
            term +=          v[pos - 9];
            v[pos] = term;
        }
        len = (size_t)n;
    }

    return v[n - 1];
}

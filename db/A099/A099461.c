/*
 * A099461  An Alexander sequence for the knot 9_48.
 * https://oeis.org/A099461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A099460(n) - A099460(n-2) Recurrence derived from G.f.:
 *            (1-x)*(1+x)*(1+x^2)/(1-7x+11x^2-7x^3+x^4) For n ≥ 5: a(n) =
 *            7a(n-1) - 11a(n-2) + 7a(n-3) - a(n-4) For n = 4: a(4) = 7a(3) -
 *            11a(2) + 7a(1) - a(0) - 1 Initial values: a(0)=1, a(1)=7,
 *            a(2)=38, a(3)=196, a(4)=1001
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation table */
    static seq_t *tbl = NULL;
    static size_t tbl_len = 0;   /* number of elements stored */
    static size_t tbl_cap = 0;   /* allocated capacity */

    /* initial table if first call */
    if (tbl_len == 0) {
        tbl_cap = 16;
        tbl = (seq_t *)malloc(tbl_cap * sizeof(seq_t));
        if (!tbl) return (seq_t)0; /* allocation failure */
        tbl[0] = (seq_t)1;
        tbl[1] = (seq_t)7;
        tbl[2] = (seq_t)38;
        tbl[3] = (seq_t)196;
        tbl[4] = (seq_t)1001;
        tbl_len = 5;
    }

    /* extend table up to n if needed */
    if (n >= tbl_len) {
        size_t need = (size_t)n + 1;
        if (need > tbl_cap) {
            /* grow in powers of two */
            while (tbl_cap < need)
                tbl_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(tbl, tbl_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0; /* allocation failure */
            tbl = tmp;
        }
        for (size_t i = tbl_len; i <= (size_t)n; ++i) {
            seq_t v;
            if (i < 5) {
                /* already stored */
                v = tbl[i];
            } else {
                /* recurrence */
                seq_t t1 = (seq_t)7 * tbl[i-1];
                seq_t t2 = (seq_t)11 * tbl[i-2];
                seq_t t3 = (seq_t)7 * tbl[i-3];
                seq_t t4 = tbl[i-4];
                v = t1 + t3;        /* positive terms first */
                v = v - t2;         /* subtract 11*a(i-2) */
                v = v - t4;         /* subtract a(i-4) */
                if (i == 4) {       /* special adjustment for a(4) */
                    v = v - (seq_t)1;
                }
            }
            tbl[i] = v;
        }
        tbl_len = (size_t)n + 1;
    }

    return tbl[(size_t)n];
}

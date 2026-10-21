/*
 * A063331  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          58 ).
 * https://oeis.org/A063331
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n > 13 Starting values (2 ≤
 *            n ≤ 13): a(2) = -1 , a(3) = 34 , a(4) = 70 , a(5) = 105 , a(6) =
 *            140 , a(7) = 175, a(8) = 210, a(9) = 243, a(10) = 280, a(11) =
 *            313, a(12) = 350, a(13) = 385
 * Range:     i8 n <= 5, i16 n <= 46, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) /* values below the offset are unused */
        return (seq_t)0;

    /* static cache of computed terms */
    static seq_t *tbl = NULL;
    static size_t sz   = 0;     /* number of elements allocated */
    static idx_t  maxi = 0;     /* largest index that has been computed */

    /* Allocate or grow the cache if needed */
    if ((size_t)n >= sz) {
        size_t new_sz = sz ? sz << 1 : 16;
        while (new_sz <= (size_t)n) new_sz <<= 1;
        seq_t *tmp = (seq_t*)realloc(tbl, new_sz * sizeof(seq_t));
        if (!tmp) {               /* allocation failed – return 0 (undefined) */
            tbl = NULL;
            sz  = 0;
            return (seq_t)0;
        }
        tbl = tmp;
        sz  = new_sz;
    }

    /* Initial values 2 … 13 – filled only once */
    static const seq_t init[14] = {
        (seq_t)0, (seq_t)0,                     /* 0, 1 (unused) */
        (seq_t)-1, (seq_t)34, (seq_t)70, (seq_t)105, (seq_t)140,
        (seq_t)175,(seq_t)210,(seq_t)243,(seq_t)280,
        (seq_t)313,(seq_t)350,(seq_t)385
    };

    if (maxi < 13) {
        idx_t start = maxi + 1;
        if (start < 2) start = 2;
        for (idx_t i = start; i <= 13 && i <= n; ++i)
            tbl[i] = init[i];
        maxi = (13 <= n) ? 13 : n;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = maxi + 1; i <= n; ++i) {
        if (i > 13)
            tbl[i] = tbl[i-4] + tbl[i-6] - tbl[i-10];
        /* values for i ≤ 13 are already set */
    }
    maxi = n;

    return tbl[n];
}

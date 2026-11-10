/*
 * A091537  Alternating row sums of array A091534 (generalized Stirling2 array
 *          (5,2)).
 * https://oeis.org/A091537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=2..2n} (-1)^k * A091534(n,k), where A091534(n,k) =
 *            Sum_{p=0..2} binomial(2,p)*fallfac(3*(n-1)+k-p,
 *            2-p)*A091534(n-1,k-p), A091534(1,2) = 1 (other entries zero);
 *            a(1) = 1
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 9, i128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *sums = NULL;   /* sums[1..count] */
    static idx_t count = 0;
    static seq_t *row = NULL;    /* current row, indexed by k = 0..2*cur */
    static idx_t cur = 0;

    if (count == 0) {
        row = mem_new(seq_t, 3);
        row[2] = (seq_t)1;
        cur = 1;
        sums = mem_new(seq_t, 2);
        sums[1] = (seq_t)1;
        count = 1;
    }

    while (count < n) {
        idx_t nr = cur + 1;
        idx_t len = 2 * nr + 1;
        seq_t *nx = mem_new(seq_t, len);
        seq_t s = (seq_t)0;
        for (idx_t k = 2; k <= 2 * nr; ++k) {
            seq_t st = (seq_t)(3 * cur + k);
            seq_t term = (seq_t)0;
            if (k <= 2 * cur)
                term += (st * (st - 1)) * row[k];
            if (k - 1 >= 2 && k - 1 <= 2 * cur)
                term += (seq_t)2 * (st - 1) * row[k - 1];
            if (k - 2 >= 2 && k - 2 <= 2 * cur)
                term += row[k - 2];
            nx[k] = term;
            if (k % 2 == 0) s += term; else s -= term;
        }
        mem_free(row);
        row = nx;
        cur = nr;
        sums = mem_resize(seq_t, sums, (idx_t)(count + 2));
        sums[count + 1] = s;
        count++;
    }
    return sums[n];
}

/*
 * A085814  Even entries (A048967) minus the odd entries (A001316) in row n of
 *          Pascal's triangle (A007318).
 * https://oeis.org/A085814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{i=0..n} (-1)^{C(n,i)} * C(n,i)
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 29, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t sumOdd = 0;

    /* enumerate all submasks of n (i.e. indices i with i&~n==0) */
    idx_t sub = n;
    while (1) {
        /* compute C(n, sub) using the multiplicative formula */
        seq_t bin = 1;
        for (idx_t t = 1; t <= sub; ++t) {
            bin = bin * (seq_t)(n - t + 1) / (seq_t)t;
        }
        sumOdd += bin;

        if (sub == 0)
            break;
        sub = (sub - 1) & n;
    }

    /* 2^n */
    seq_t pow2n = pow_seq((seq_t)2, n);

    /* a(n) = 2^n - 2*sumOdd */
    seq_t res = pow2n - (sumOdd * (seq_t)2);
    return res;
}

/*
 * A023983  Sum of exponents in prime-power factorization of multinomial
 *          coefficient M(5n;2n,2n,n).
 * https://oeis.org/A023983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Ω((5n)!) – 2·Ω((2n)!) – Ω(n!)
 * Range:     u8 n <= 276, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Ω(m) = total number of prime factors of m, counted with multiplicity. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    idx_t five = 5 * n;
    idx_t two  = 2 * n;

    seq_t sum5 = 0;   /* Ω((5n)!) */
    seq_t sum2 = 0;   /* Ω((2n)!) */
    seq_t sum1 = 0;   /* Ω(n!)   */

    for (idx_t i = 1; i <= five; ++i) {
        seq_t w = bigomega(i);      /* Ω(i) */

        sum5 += w;
        if (i <= two) sum2 += w;
        if (i <= n)   sum1 += w;
    }

    return sum5 - (seq_t)2 * sum2 - sum1;
}

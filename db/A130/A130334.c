/*
 * A130334  Smallest m>n such that the m-th and n-th triangular numbers are
 *          coprime.
 * https://oeis.org/A130334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m > n such that gcd(T_m , T_n) = 1 where T_k =
 *            k*(k+1)/2 is the k-th triangular number
 * Range:     u8 n <= 20, u16 n <= 734, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Compute T_n as seq_t */
    seq_t Tn = ((seq_t)n * ((seq_t)n + 1)) / 2;

    idx_t m = n + 1;
    while (1) {
        seq_t Tm = ((seq_t)m * ((seq_t)m + 1)) / 2;
        if (gcd(Tn, Tm) == 1) {
            /* Return m as seq_t */
            return (seq_t)m;
        }
        ++m;
    }
}

/*
 * A226429  Number of numerical semigroups S of genus 3^n having a minimal
 *          generating set of cardinality 2.
 * https://oeis.org/A226429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = number of unordered coprime pairs (x,y) with * x<y and
 *            (x-1)(y-1) = 2*3^n
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* compute 3^n as big integer */
    seq_t pow3_n = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow3_n = pow3_n * (seq_t)3;

    seq_t pow3_f = (seq_t)1;
    size_t count = 0;
    idx_t half = n / 2;

    for (idx_t f = 0; f <= half; ++f) {
        seq_t pow3_n_minus_f = pow3_n / pow3_f;

        /* e = 0 */
        seq_t d0 = pow3_f;
        seq_t B0 = pow3_n_minus_f * (seq_t)2;
        if (d0 <= B0) {
            seq_t g = gcd(d0 + (seq_t)1, B0 + (seq_t)1);
            if (g == (seq_t)1)
                ++count;
        }

        /* e = 1 */
        seq_t d1 = pow3_f * (seq_t)2;
        seq_t B1 = pow3_n_minus_f;
        if (d1 <= B1) {
            seq_t g = gcd(d1 + (seq_t)1, B1 + (seq_t)1);
            if (g == (seq_t)1)
                ++count;
        }

        pow3_f = pow3_f * (seq_t)3;
    }

    return (seq_t)count;
}

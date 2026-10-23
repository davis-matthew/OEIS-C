/*
 * A030695  Smallest nontrivial extension of n-th cube which is a cube.
 * https://oeis.org/A030695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest cube k³ such that k>n and decimal digits of k³
 *            start with n³
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* target cube n³ */
    seq_t target = (seq_t)n * (seq_t)n * (seq_t)n;

    /* number of decimal digits of target */
    size_t target_digits = 0;
    seq_t t = target;
    while (t > 0) {
        t /= 10;
        target_digits++;
    }

    /* search for the smallest k>n */
    for (idx_t k = n + 1; ; ++k) {
        seq_t k3 = (seq_t)k * (seq_t)k * (seq_t)k;

        /* number of digits of k³ */
        size_t k3_digits = 0;
        seq_t u = k3;
        while (u > 0) {
            u /= 10;
            k3_digits++;
        }

        /* leading `target_digits` digits of k³ */
        size_t shift = k3_digits - target_digits;
        seq_t divisor = 1;
        for (size_t i = 0; i < shift; ++i) divisor *= 10;
        seq_t leading = k3 / divisor;

        if (leading == target) return k3;
    }
}

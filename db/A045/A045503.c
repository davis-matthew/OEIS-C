/*
 * A045503  If decimal expansion of n is ab...d, a(n) = a^a + b^b +...+ d^d.
 * https://oeis.org/A045503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ d_i^{d_i} for each decimal digit d_i of n, with 0^0
 *            defined as 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

/* Compute the sum of each decimal digit raised to its own power.
   0^0 is treated as 1. */
seq_t a(idx_t n)
{
    static seq_t powValues[10];
    static bool initialized = false;

    if (!initialized) {
        powValues[0] = (seq_t)1;                /* 0^0 = 1 */
        for (int d = 1; d <= 9; ++d)
            powValues[d] = big_pow((seq_t)d, (seq_t)d);
        initialized = true;
    }

    /* Handle n == 0 explicitly: 0 has a single digit 0. */
    if (n == 0)
        return powValues[0];

    seq_t sum = 0;
    while (n > 0) {
        int digit = (int)(n % 10);      /* digit in [0,9] */
        sum += powValues[digit];
        n /= 10;
    }
    return sum;
}

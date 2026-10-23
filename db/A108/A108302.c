/*
 * A108302  Concatenate n and the sum of the digits of n raised to their own
 *          power (A045503).
 * https://oeis.org/A108302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concat(n, Σ over digits d of n (d^d)), with 0^0 = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t pow_tab[10] = {
    /* 0^0, 1^1, 2^2, ..., 9^9 */
    1ull, 1ull, 4ull, 27ull, 256ull, 3125ull,
    46656ull, 823543ull, 16777216ull, 387420489ull
};

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0) {
        /* For the digit 0, use 0^0 = 1 */
        sum = pow_tab[0];
    } else {
        idx_t temp = n;
        while (temp > 0) {
            unsigned d = (unsigned)(temp % 10);
            sum += pow_tab[d];
            temp /= 10;
        }
    }

    /* Count digits of sum */
    unsigned digits = 0;
    seq_t tmp = sum;
    do {
        ++digits;
        tmp /= 10;
    } while (tmp != 0);

    /* Compute 10^digits */
    seq_t pow10 = 1;
    for (unsigned i = 0; i < digits; ++i) {
        pow10 *= 10;
    }

    /* Concatenate n and sum */
    return ((seq_t)n) * pow10 + sum;
}

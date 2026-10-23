/*
 * A109847  Least common multiple of n and its 9's complement.
 * https://oeis.org/A109847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = lcm(n, 9's complement of n)
 * Range:     u8 n <= 9, u16 n <= 99, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean algorithm for seq_t */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* compute the 9's complement of n */
    seq_t comp = 0;
    seq_t factor = 1;
    idx_t temp = n;
    while (temp > 0) {
        idx_t digit = temp % 10;
        temp /= 10;
        comp += (seq_t)(9 - digit) * factor;
        factor *= 10;
    }

    /* if the complement is 0, lcm(0, n) = 0 */
    if (comp == 0)
        return 0;

    seq_t N = (seq_t)n;
    seq_t g = gcd_seq(N, comp);
    seq_t lcm = (N / g) * comp;
    return lcm;
}

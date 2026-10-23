/*
 * A008659  Theta series of direct sum of 3 copies of D_4 lattice.
 * https://oeis.org/A008659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 72 * σ₅(n) for odd n * a(n) = 72 * σ₅(n) – 576 * σ₅(n/2)
 *            for even n * where σ₅(m) is the sum of fifth powers of the
 *            divisors of m. * a(0) is defined to be 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 36, u64 n <= 3032,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Sum of fifth powers of the divisors of m. */
static seq_t sigma5(idx_t m)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= m / i; ++i) {
        if (m % i == 0) {
            seq_t d1 = (seq_t)i;
            seq_t d1pow = d1 * d1 * d1 * d1 * d1;
            sum += d1pow;

            idx_t j = m / i;
            if (j != i) {
                seq_t d2 = (seq_t)j;
                seq_t d2pow = d2 * d2 * d2 * d2 * d2;
                sum += d2pow;
            }
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t s = sigma5(n);

    if ((n & 1) == 0) {          /* n is even */
        seq_t sh = sigma5(n >> 1);
        return s * 72 - sh * 576;
    } else {                      /* n is odd */
        return s * 72;
    }
}

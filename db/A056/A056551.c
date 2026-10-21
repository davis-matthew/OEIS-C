/*
 * A056551  Smallest cube divisible by n divided by largest cube which divides
 *          n.
 * https://oeis.org/A056551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n, e mod 3 ≠ 0} p³
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) computes the smallest cube divisible by n divided by the largest cube dividing n. */

#include <stdint.h>  /* for uint64_t if needed, does not affect the build */

seq_t a(idx_t n)
{
    /* Handle the offset and the trivial case. */
    if (n <= 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t temp = n;

    /* Trial division by all integers up to sqrt(temp). */
    for (idx_t d = 2; d <= temp / d; ++d) {
        if (temp % d == 0) {
            idx_t exp = 0;
            while (temp % d == 0) {
                temp /= d;
                ++exp;
            }
            if (exp % 3 != 0) {
                seq_t p = (seq_t)d;
                /* Multiply by p³; overflows in small builds are acceptable
                   because the sequence values stay within the target type. */
                result *= (p * p * p);
            }
        }
    }

    /* If a prime remains after the loop, its exponent is 1 (≠0 mod 3). */
    if (temp > 1) {
        seq_t p = (seq_t)temp;
        result *= (p * p * p);
    }

    return result;
}

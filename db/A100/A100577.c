/*
 * A100577  Number of sets of divisors of n with an odd sum.
 * https://oeis.org/A100577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(A000005(n)-1) (A000005(n) = number of divisors of n)
 * Range:     u8 n <= 35, u16 n <= 179, u32 n <= 1259, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) /* offset handling; sequence is defined for n >= 1 */
        return 0;

    idx_t tmp = n;
    uint64_t d = 1;            /* number of divisors */
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            d *= (exp + 1);
        }
    }
    if (tmp > 1)              /* remaining prime factor */
        d *= 2;

    seq_t result = 1;
    for (uint64_t i = 1; i < d; ++i)   /* multiply by 2, d-1 times */
        result = result * (seq_t)2;

    return result;
}

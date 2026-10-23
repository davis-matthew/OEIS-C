/*
 * A015995  a(n) = (tau(n^3)+2)/3.
 * https://oeis.org/A015995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (tau(n^3)+2)/3 where tau(m) is the number of divisors of
 *            m. The implementation factorises n, computes
 *            tau(n^3)=∏(3*e_i+1), then returns (tau+2)/3
 * Range:     u8 n <= 1259, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* factorise n by trial division */
    idx_t temp = n;
    seq_t tau = 1;

    /* handle factor 2 */
    if (temp % 2 == 0) {
        uint64_t exp = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            ++exp;
        }
        tau *= (seq_t)(3 * exp + 1);
    }

    /* handle odd factors */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            tau *= (seq_t)(3 * exp + 1);
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (temp > 1) {
        tau *= (seq_t)(3 * 1 + 1);   /* exponent is 1 */
    }

    /* return (tau + 2)/3 */
    return (tau + 2) / 3;
}

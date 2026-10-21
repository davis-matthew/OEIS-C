/*
 * A075857  Least common multiple of totient and cototient of n.
 * https://oeis.org/A075857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(phi(n), n - phi(n))
 * Range:     u8 n <= 32, u16 n <= 560, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;          /* φ(1) = 1, cototient = 0, lcm = 0 */

    /* Compute Euler's totient φ(n) */
    seq_t phi_val = (seq_t)n;
    idx_t tmp = n;

    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            while (tmp % p == 0)
                tmp /= p;
            phi_val -= phi_val / (seq_t)p;
        }
    }
    if (tmp > 1) {
        phi_val -= phi_val / (seq_t)tmp;
    }

    /* Cototient = n - φ(n) */
    seq_t cotot = (seq_t)n - phi_val;

    /* lcm(φ(n), cotot) = φ(n) / gcd(φ(n), cotot) * cotot */
    seq_t g = gcd(phi_val, cotot);
    seq_t lcm = (phi_val / g) * cotot;

    return lcm;
}

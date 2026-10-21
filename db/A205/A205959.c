/*
 * A205959  a(n) = n^omega(n)/rad(n).
 * https://oeis.org/A205959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Product_{p|n} n/p
 * Range:     u8 n <= 29, u16 n <= 149, u32 n <= 1049, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) is 1 by definition */
    if (n <= 1)
        return 1;

    /* Use 64‑bit arithmetic for the factorisation */
    uint64_t nn = n;
    uint64_t temp = nn;
    seq_t prod = 1;

    /* handle factor 2 explicitly */
    if (temp % 2 == 0) {
        prod *= (seq_t)nn / 2;
        while (temp % 2 == 0)
            temp /= 2;
    }

    /* check odd factors up to sqrt(temp) */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            prod *= (seq_t)nn / (seq_t)p;
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* any remaining prime factor > sqrt(nn) */
    if (temp > 1)
        prod *= (seq_t)nn / (seq_t)temp;

    return prod;
}

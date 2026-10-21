/*
 * A089631  a(n) = (Product_{p is a prime factor of n} p) mod (Product_{p is a
 *          prime factor of n} p-1).
 * https://oeis.org/A089631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Π_{p|n} p) mod (Π_{p|n} (p - 1))
 * Range:     u8 n <= 554, u16 n <= 9029, u32 n <= 9029, u64 n <= 9029,
 *            u128 n <= 9029, big any n
 * Helpers:   factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                      // a(1)=0 (empty product)

    uint64_t pr[64], exp[64];
    size_t cnt = 0;
    factor((uint64_t)n, pr, exp, &cnt); // factor n into distinct primes

    seq_t rad = 1;          // product of distinct primes
    seq_t phi_rad = 1;      // product of (p - 1)

    for (size_t i = 0; i < cnt; ++i) {
        rad      *= (seq_t)pr[i];
        phi_rad  *= (seq_t)(pr[i] - 1);
    }

    return rad % phi_rad;   // rad mod phi_rad
}

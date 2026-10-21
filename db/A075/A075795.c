/*
 * A075795  Number of k, 0<k<=n, such that the resultant of the k-th
 *          cyclotomic polynomial and the n-th cyclotomic polynomial is equal
 *          to 1.
 * https://oeis.org/A075795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - 1 - Ω(n) (Ω(n) = total number of prime factors of n,
 *            counted with multiplicity)
 * Range:     u8 n <= 258, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n <= 1) return 0;                // a(1) = 0

    uint64_t tmp = (uint64_t)n;
    seq_t omega = 0;

    /* count factors 2 */
    while ((tmp & 1UL) == 0) {
        ++omega;
        tmp >>= 1;
    }

    /* count odd factors */
    for (uint64_t p = 3; ( __uint128_t)p * p <= tmp; p += 2) {
        while (tmp % p == 0) {
            ++omega;
            tmp /= p;
        }
    }

    if (tmp > 1) {            /* remaining prime factor > sqrt(original n) */
        ++omega;
    }

    return (seq_t)n - 1 - omega;
}

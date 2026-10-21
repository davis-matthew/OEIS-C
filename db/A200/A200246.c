/*
 * A200246  a(n)=1 iff binary weight of n-th prime is odd.
 * https://oeis.org/A200246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = 1 iff the binary weight of the n‑th prime is odd. */
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* out of bounds, return 0 */
    uint32_t p = get_prime(n);           /* nth prime (n ≥ 1) */
    unsigned parity = 0;                 /* 0 = even, 1 = odd */
    while (p) {
        parity ^= 1;                     /* flip parity each set bit */
        p &= p - 1;                      /* clear lowest set bit */
    }
    return (seq_t)parity;
}

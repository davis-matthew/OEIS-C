/*
 * A053671  Least number coprime to n, n+1 and n+2.
 * https://oeis.org/A053671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the least integer m ≥ 2 such that gcd(m,n) = gcd(m,n+1) = gcd(m,n+2) = 1. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd_u64.h"

/* Euclidean algorithm for 64‑bit unsigned integers.  */

seq_t a(idx_t n)
{
    /* Convert the index to 64‑bit unsigned for arithmetic.  */
    uint64_t n0 = n;
    uint64_t n1 = n0 + 1;
    uint64_t n2 = n0 + 2;

    /* Search for the smallest m ≥ 2 that is coprime to all three numbers. */
    for (uint64_t m = 2;; ++m) {
        if (gcd_u64(m, n0) == 1 &&
            gcd_u64(m, n1) == 1 &&
            gcd_u64(m, n2) == 1) {
            return (seq_t)m;   /* cast to the defined output type */
        }
    }
}

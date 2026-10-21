/*
 * A098388  a(n) = floor(log_2(prime(n))).
 * https://oeis.org/A098388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(log2(prime(n)))
 * Range:     u8 n <= 54, u16 n <= 7180, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* For n >= 1 this returns floor(log2 of the nth prime). */
    if (n == 0) {
        return (seq_t)0;        /* out of range, return 0 */
    }
    /* Obtain the nth prime (offset 1).  get_prime(1) == 2. */
    seq_t p = (seq_t)get_prime(n);

    /* Compute floor(log2(p)).  p can safely fit into uint64_t for all n
       that we test (e.g. n <= 10000 → p <= 104729). */
    unsigned log2p = floor_log2_uint64((uint64_t)p);

    return (seq_t)log2p;
}

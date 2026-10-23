/*
 * A177959  n-th prime minus number of 0's in binary representation of n-th
 *          prime.
 * https://oeis.org/A177959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth prime minus number of 0's in binary representation of
 *            nth prime
 * Range:     u8 n <= 54, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   bitlen_seq_t, get_prime, popcount_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/popcount_u64.h"
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* Offset starts at 1: a(1) is the first term */
    if (n == 0)
        return (seq_t)0;

    uint32_t p = get_prime(n);            /* nth prime, 1‑indexed */
    unsigned ones  = (unsigned)popcount_u64((uint64_t)p);
    unsigned total = bitlen_seq_t((seq_t)p);
    unsigned zeros = total - ones;
    return (seq_t)p - (seq_t)zeros;
}

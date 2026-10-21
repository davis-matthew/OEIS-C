/*
 * A098391  a(n) = Log2(Log2(prime(n))), where Log2 = A000523.
 * https://oeis.org/A098391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(log2(floor(log2(prime(n)))))
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   floor_log2_uint64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                           /* offset handling */

    /* Obtain the nth prime (fits in uint32_t for n up to at least 10000). */
    uint64_t p = (uint64_t)get_prime(n);

    /* First logarithm: floor(log2(p)). */
    idx_t l1 = floor_log2_uint64(p);

    /* Second logarithm: floor(log2(l1)). */
    idx_t l2 = floor_log2_uint64(l1);

    return (seq_t)l2;
}

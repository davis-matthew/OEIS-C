/*
 * A101871  Number of Abelian groups of order 2n+1.
 * https://oeis.org/A101871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000688(2*n+1) = product_{p^e||2n+1} partition(e)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   mul128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           where partition(e) is the number of integer partitions of e
 *           (partition(0)=1, partition(1)=1, partition(2)=2, …) */

#include <stdint.h>
#include "../utils/mul128.h"

static const seq_t partition_table[10] = {
    1, /* partition(0) */
    1, /* partition(1) */
    2, /* partition(2) */
    3, /* partition(3) */
    5, /* partition(4) */
    7, /* partition(5) */
    11,/* partition(6) */
    15,/* partition(7) */
    22,/* partition(8) */
    30 /* partition(9) */
};

seq_t a(idx_t n)
{
    /* Handle the trivial case n=0 separately (order 1) */
    if (n == 0) return 1;

    uint64_t m = 2 * (uint64_t)n + 1;        /* the odd order to factor */
    seq_t result = 1;
    uint64_t temp = m;

    /* trial division by odd primes up to sqrt(temp) */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* e <= 9 for m <= 20001, safe to index table */
            result = mul128(result, partition_table[e]);
        }
    }

    /* If a prime factor larger than sqrt(temp) remains, it has exponent 1 */
    if (temp > 1) {
        /* partition(1) == 1, multiplication is harmless */
        result = mul128(result, partition_table[1]);
    }

    return result;
}

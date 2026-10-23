/*
 * A127992  Number of distinct prime factors of 2*n^3 - 2*n + 9.
 * https://oeis.org/A127992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime factors of (2*n^3 - 2*n + 9)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* Undefined for n < 1, return 0 for safety */

    uint64_t nn = (uint64_t)n;

    /* Compute 2*n^3 - 2*n + 9 using 128–bit arithmetic to avoid overflow */
    unsigned __int128 val128 = (unsigned __int128)2 * nn * nn * nn
                             - (unsigned __int128)2 * nn
                             + (unsigned __int128)9;

    /* For n up to about 10000, val128 fits in uint64_t */
    uint64_t val = (uint64_t)val128;

    /* Use the provided helper to count distinct prime factors */
    int res = count_distinct_primes(val);

    return (seq_t)res;
}

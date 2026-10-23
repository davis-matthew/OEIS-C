/*
 * A147810  Half the number of divisors of n^2+1.
 * https://oeis.org/A147810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A147810: Half the number of divisors of n^2+1
 *  Formula: a(n) = d(n^2+1)/2
 *  where d(m) is the number of (positive) divisors of m.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* a(1) corresponds to n = 1, as required by OEIS offset. */
    if (n == 0)
        return 0;

    /* Compute n^2 + 1, staying within 64‑bit limits for the test range. */
    uint64_t x = (uint64_t)n;
    uint64_t m = x * x + 1ULL;          /* n^2+1 */

    /* Count divisors using the provided utility routine. */
    uint64_t dcnt = divisor_count_u64(m);

    /* a(n) = d(n^2+1)/2.  The division is exact because n^2+1 is not a
     * square, so d(n^2+1) is even. */
    return (seq_t)(dcnt / 2ULL);
}

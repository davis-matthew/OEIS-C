/*
 * A140965  Smallest multiple of n with a number of divisors >= n.
 * https://oeis.org/A140965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest multiple of n whose number of divisors ≥ n
 *            (search by trial multiples)
 * Range:     u8 n <= 12, u16 n <= 66, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Work with 64‑bit integers – all results for n ≤ 10000 fit in 64 bits.
       The cast to seq_t is safe because seq_t can hold at least 64 bits. */
    uint64_t step = (uint64_t)n;
    uint64_t m = step;          /* start from the first multiple n */

    while (divisor_count_u64(m) < n)
        m += step;

    return (seq_t)m;
}

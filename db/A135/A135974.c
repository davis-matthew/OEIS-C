/*
 * A135974  a(n) = the smallest integer m > n such that d(m) > d(n), where
 *          d(n) = number of divisors of n.
 * https://oeis.org/A135974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   compute the smallest integer m > n such that d(m) > d(n), where
 *            d(k) is the number of positive divisors of k
 * Range:     u8 n <= 239, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a simple brute‑force search combined
   with the helper function divisor_count_u64() to obtain d(k). */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Compute divisor count of the starting index */
    uint64_t dn = divisor_count_u64((uint64_t)n);

    /* Search forward until we find a number with more divisors */
    for (idx_t m = n + 1; ; ++m)
    {
        uint64_t dm = divisor_count_u64((uint64_t)m);
        if (dm > dn)
            return (seq_t)m;
    }
}

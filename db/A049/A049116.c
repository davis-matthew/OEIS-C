/*
 * A049116  Repeatedly apply Euler phi to n; a(n) = highest power of 2 that is
 *          seen.
 * https://oeis.org/A049116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum power of 2 appearing in the iterated phi sequence
 *            starting at n. The function repeatedly applies Euler's totient
 *            function phi to n until reaching 1, keeping track of the largest
 *            power of two encountered
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 108 terms through n = 9998 (2026-11-01)
 * Helpers:   is_pow2, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_pow2.h"
#include "../utils/totient_seq.h"

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    seq_t max = 0;

    while (1)
    {
        if (is_pow2(m) && m > max)
            max = m;

        if (m == (seq_t)1)
            break;

        m = totient_seq(m);
    }

    return max;
}

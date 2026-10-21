/*
 * A226778  Numbers having no common divisor > 1 with their reversal in
 *          decimal representation (see A043537).
 * https://oeis.org/A226778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 5281, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:  A226778 – numbers having no common divisor > 1 with their decimal
   reversal.  Computation: iterate integers and return the n‑th number
   x such that gcd(x, reverse_decimal(x)) == 1. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/rev_decimal.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t count = 0;
    idx_t m = 1;

    for (;; ++m)
    {
        seq_t val = (seq_t)m;
        seq_t rev_val = rev_decimal(val);
        if (gcd(val, rev_val) == (seq_t)1)
        {
            ++count;
            if (count == n)
                return val;
        }
    }
}

/*
 * A029881  Möbius transform of Thue-Morse sequence A001285 (when shifted once
 *          right).
 * https://oeis.org/A029881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 20000, i16 n <= 20000, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 117 terms through n = 9866 (2026-11-01)
 * Helpers:   mobius_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula implemented: a(n) = Sum_{d|n} mu(n/d) * (A001285(d-1) + 1)
   where A001285(k) is the Thue‑Morse bit (0 for even popcount, 1 for odd),
   and mu is the Möbius function.
   The added "+1" turns the 0/1 values into 1/2, matching the
   OEIS sequence A029881. */
 
#include <stdint.h>
#include <stddef.h>
#include "../utils/mobius_idx.h"

/* Thue‑Morse value T(k): 0 if popcount(k) even, 1 if odd. */
static inline unsigned thue_morse(idx_t k)
{
    return __builtin_popcountll(k) & 1u;
}

/* Möbius transform of the shifted Thue‑Morse sequence. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            /* divisor d, corresponding factor n/d */
            idx_t d1 = d;
            idx_t d2 = n / d;

            int mu_d2 = mobius_idx(d2);
            unsigned tm1 = thue_morse(d1 - 1ull);
            sum += (seq_t)mu_d2 * (seq_t)(tm1 + 1);

            if (d1 != d2)          /* avoid double counting perfect square */
            {
                int mu_d1 = mobius_idx(d1);
                unsigned tm2 = thue_morse(d2 - 1ull);
                sum += (seq_t)mu_d1 * (seq_t)(tm2 + 1);
            }
        }
    }
    return sum;
}

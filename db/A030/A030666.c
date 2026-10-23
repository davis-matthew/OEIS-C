/*
 * A030666  Smallest nontrivial extension of n which is a square.
 * https://oeis.org/A030666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest perfect square that can be obtained by appending
 *            at least one decimal digit to n
 * Range:     u8 n <= 4, u16 n <= 34, u32 n <= 9948, u64 n <= 9948,
 *            u128 n <= 9948, big any n
 * Helpers:   floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* For each n (n >= 1) look for the smallest k >= 1 such that
       there exists an integer m with m² in the interval
         [ n * 10^k , (n + 1) * 10^k – 1 ].
       The smallest such m gives the required square. */
    for (idx_t k = 1; ; ++k) {
        seq_t pow10e = pow10(k);                     // 10^k as seq_t
        seq_t L = (seq_t)n * pow10e;                 // lower bound of interval
        seq_t R = ((seq_t)(n + 1) * pow10e) - 1;     // upper bound

        seq_t low  = floor_sqrt(L);                  // floor √L
        if (low * low < L)                           // adjust to ceil √L
            ++low;

        seq_t high = floor_sqrt(R);                  // floor √R

        if (low <= high) {                           // non‑empty interval
            return low * low;                        // smallest square
        }
    }
}

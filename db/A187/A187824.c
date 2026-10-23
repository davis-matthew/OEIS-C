/*
 * A187824  a(n) is the largest m such that n is congruent to -1, 0 or 1 mod k
 *          for all k from 1 to m.
 * https://oeis.org/A187824
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = maximum m such that for all k = 1..m, n mod k ∈ {0, 1,
 *            k-1}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n >= 2.  For n < 2 the function returns 0 to avoid
   an infinite loop (since n = 0 would satisfy the condition for all k).
   Because the largest possible m for a 64‑bit n is under 30, the
   algorithm runs in O(m) time, which is far below the 2‑second limit
   even for thousands of calls. */
#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 2)
        return 0;                  /* undefined for n < 2, safe fallback */

    idx_t m = 0;
    for (idx_t k = 1;; ++k) {
        idx_t r = n % k;
        if (r == 0 || r == 1 || r == k - 1)
            ++m;                  /* still valid, so increment m */
        else
            break;                /* condition fails */
    }
    return (seq_t)m;
}

/*
 * A054424  Permutation of natural numbers: maps the canonical list of
 *          fractions (A020652/A020653) to whole Stern-Brocot (Farey) tree
 *          (top = 1/1 and both sides < 1 and > 1, but excluding the
 *          "fractions" 0/1 and 1/0).
 * https://oeis.org/A054424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = heap (breadth‑first left‑to‑right) index of the n-th
 *            reduced fraction when fractions are ordered by increasing sum
 *            a+b and, for equal sums, by increasing a
 * Range:     u8 n <= 27, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   compute_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/compute_index.h"

static inline uint64_t gcd_idx(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* n < 1 is outside the sequence; return 0 as a placeholder */

    uint64_t count = 0;
    for (uint64_t sum = 2; ; ++sum) {
        for (uint64_t a = 1; a < sum; ++a) {
            uint64_t b = sum - a;
            if (gcd_idx(a, b) != 1) continue;   /* skip non‑reduced fractions */
            ++count;
            if (count == n) {
                return compute_index(a, b);
            }
        }
    }
}

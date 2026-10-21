/*
 * A055569  Sum of digits of a(n)^3 is greater than or equal to a(n).
 * https://oeis.org/A055569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) is the n‑th non‑negative integer k such that sum of the
 *            decimal digits of k³ is ≥ k. The sequence terminates at k = 27,
 *            but the routine searches up to a sensible upper limit
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Search from 0 upward until the n‑th qualifying number is found.
       The sequence stops at 27; for n beyond the last term return 0. */
    const idx_t LIMIT = 1000000;   /* arbitrary search ceiling */
    idx_t candidate = 0;
    idx_t count = 0;

    for (;; ++candidate) {
        if (candidate > LIMIT) return 0;          /* no more terms */
        seq_t sq = (seq_t)candidate;
        seq_t cube = sq * sq * sq;                /* k³ */
        seq_t sum = digit_sum(cube);              /* sum of digits of k³ */
        if (sum >= sq) {
            if (count == n) return sq;            /* found the n‑th term */
            count++;                              
        }
    }
}

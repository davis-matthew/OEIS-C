/*
 * A092280  Partial sums of periods of reciprocals.
 * https://oeis.org/A092280
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = Σ_{k=2}^n period_of(k) where period_of(k) = 0 if k has
 *            only factors 2 or 5; otherwise let m = k with all 2's and 5's
 *            removed, then period_of(k) = multiplicative order of 10 modulo m
 * Range:     u8 n <= 45, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   period_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/period_of.h"

seq_t a(idx_t n)
{
    if (n < 2) {
        return (seq_t)0;   /* offset starts at 2 */
    }

    static seq_t *cum = NULL;
    static size_t cap = 0;

    if ((size_t)n >= cap) {
        size_t old_cap = cap;
        size_t new_cap = (size_t)n + 1;

        cum = (seq_t *)realloc(cum, new_cap * sizeof(seq_t));
        if (old_cap == 0) {
            cum[0] = (seq_t)0;
            cum[1] = (seq_t)0;
        }

        size_t start = old_cap;
        if (start < 2) {
            start = 2;
        }
        for (size_t i = start; i <= (size_t)n; ++i) {
            seq_t period = period_of((idx_t)i);
            cum[i] = cum[i - 1] + period;
        }
        cap = new_cap;
    }

    return cum[(size_t)n];
}

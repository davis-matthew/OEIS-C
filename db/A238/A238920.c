/*
 * A238920  Number of self-inverse permutations p on [n] where the maximal
 *          displacement of an element equals 9.
 * https://oeis.org/A238920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count_f(n,9) - count_f(n,8) * where count_f(n,D) counts
 *            self‑inverse permutations on [n] * whose maximal displacement is
 *            <= D
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 21, u64 n <= 40, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* count_f counts involutions on [n] with maximal displacement <= D. */
static seq_t count_f(idx_t n, unsigned int D)
{
    /* STATES = 2^D (max 512 for D <= 9) */
    const size_t STATES = (size_t)1 << D;
    seq_t dp[512];
    seq_t ndp[512];

    /* initialise dp[0] = 1, others 0 */
    for (size_t i = 0; i < STATES; ++i) dp[i] = 0;
    dp[0] = 1;

    for (idx_t pos = 1; pos <= n; ++pos) {
        /* clear ndp */
        for (size_t i = 0; i < STATES; ++i) ndp[i] = 0;

        /* maximum offset allowed from this position */
        size_t max_offset = 0;
        if (n > pos) {
            size_t diff = (size_t)(n - pos);
            max_offset = diff < D ? diff : D;
        }

        for (size_t mask = 0; mask < STATES; ++mask) {
            seq_t val = dp[mask];
            if (!val) continue;

            if (mask & 1ULL) {
                /* current position already matched by a previous transposition */
                ndp[mask >> 1] += val;
            } else {
                /* Option 1: leave current position fixed */
                ndp[mask >> 1] += val;

                /* Option 2: pair current position with a future position */
                for (size_t k = 1; k <= max_offset; ++k) {
                    if ((mask >> k) & 1ULL) continue;          /* future already matched */
                    size_t newmask = (mask | (1ULL << k)) >> 1; /* shift window after pairing */
                    ndp[newmask] += val;
                }
            }
        }

        /* move to next position */
        for (size_t i = 0; i < STATES; ++i) dp[i] = ndp[i];
    }

    /* Sum all final states */
    seq_t total = 0;
    for (size_t i = 0; i < STATES; ++i) total += dp[i];
    return total;
}

seq_t a(idx_t n)
{
    seq_t f9 = count_f(n, 9);
    seq_t f8 = count_f(n, 8);
    return f9 - f8;
}

/*
 * A164448  Number of binary strings of length n with no substrings equal to
 *          0001 0010 or 0111.
 * https://oeis.org/A164448
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 25, u32 n <= 54, u64 n <= 112,
 *            u128 n <= 228, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A164448: DP with 8 states counting binary strings of length n that avoid
   the forbidden 4‑bit patterns 0001, 0010, 0111. */
#include <stdint.h>
#include <stddef.h>

/* Function:
 *   seq_t a(idx_t n)
 *
 * Computes the number of binary strings of length n that do NOT contain
 * the substrings 0001, 0010 or 0111.  The recurrence is implemented by
 * dynamic programming over the last three bits of a partial string.
 *
 * Complexity:  O(n) per call, with a constant factor of 16 (8 states × 2 bits).
 */
seq_t a(idx_t n)
{
    /* Two arrays for dynamic programming: current and next state counts. */
    seq_t curr[8] = {0}, next[8] = {0};

    /* Initial state: empty string with no bits. */
    curr[0] = 1;

    for (idx_t len = 0; len < n; ++len) {
        /* Reset next state counts. */
        for (int s = 0; s < 8; ++s) next[s] = 0;

        for (int s = 0; s < 8; ++s) {
            seq_t ways = curr[s];
            if (ways == 0) continue;

            for (int b = 0; b <= 1; ++b) {
                int new_s = ((s << 1) | b) & 7;
                /* If we already have 3 previous bits (len >= 3), then adding
                   the new bit creates a 4‑bit window.  Skip if that window
                   matches a forbidden pattern. */
                if (len >= 3) {
                    int pat = ((s << 1) | b);
                    if (pat == 1 || pat == 2 || pat == 7) continue;
                }
                next[new_s] += ways;
            }
        }

        /* Move to the next length. */
        for (int s = 0; s < 8; ++s) curr[s] = next[s];
    }

    /* The answer is the sum over all possible last‑3‑bit states. */
    seq_t result = 0;
    for (int s = 0; s < 8; ++s) result += curr[s];
    return result;
}

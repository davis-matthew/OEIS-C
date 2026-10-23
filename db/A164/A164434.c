/*
 * A164434  Number of binary strings of length n with no substrings equal to
 *          0000 0101 or 1010.
 * https://oeis.org/A164434
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 39, u64 n <= 78, u128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164434:
   Number of binary strings of length n with no substrings 0000, 0101 or 1010.
   Recurrence (valid for n ≥ 11):
      a(n) = a(n‑1) + 2·a(n‑3) + a(n‑4) – a(n‑6) – a(n‑7)
   Initial values for 4 ≤ n ≤ 10:
      a(4)=13, a(5)=23, a(6)=40, a(7)=70,
      a(8)=122, a(9)=213, a(10)=372
   This implementation uses a DFA with 8 states representing the last
   three bits of the current prefix.  A transition to a state is
   forbidden if the newly formed 4‑bit suffix equals 0000, 0101 or 1010.
*/

#include <stddef.h>

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 4; behaviour for smaller n is irrelevant. */
    if (n < 4) {
        return 0;
    }

    /* DP arrays: dp[st] holds the number of length‑pos strings ending in state st. */
    seq_t dp[8]     = {0};
    seq_t dp_next[8] = {0};

    /* Start with the empty string: state 0 (no bits yet). */
    dp[0] = 1;

    for (idx_t pos = 0; pos < n; ++pos) {
        /* Zero dp_next for the next iteration. */
        for (idx_t st = 0; st < 8; ++st) {
            dp_next[st] = 0;
        }

        for (idx_t st = 0; st < 8; ++st) {
            seq_t cur = dp[st];
            if (!cur) continue;

            for (int bit = 0; bit < 2; ++bit) {
                /* For the first three added bits, no forbidden 4‑bit pattern
                   can yet exist, so all transitions are allowed. */
                if (pos + 1 < 4) {
                    /* Accept transition. */
                } else {
                    /* Check if the new 4‑bit suffix is one of the forbidden patterns. */
                    uint8_t word4 = ((st << 1) | bit) & 0xFu; /* 4‑bit value */
                    if (word4 == 0x0u || word4 == 0x5u || word4 == 0xAu) {
                        continue; /* forbidden transition */
                    }
                }

                uint8_t new_state = ((st << 1) | bit) & 0x7u; /* keep only last 3 bits */
                dp_next[new_state] += cur;
            }
        }

        /* Prepare for the next position. */
        for (idx_t st = 0; st < 8; ++st) {
            dp[st] = dp_next[st];
        }
    }

    /* Sum over all states to obtain the total number of valid strings of length n. */
    seq_t result = 0;
    for (idx_t st = 0; st < 8; ++st) {
        result += dp[st];
    }

    return result;
}

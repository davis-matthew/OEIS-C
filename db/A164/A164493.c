/*
 * A164493  Number of binary strings of length n with no substrings equal to
 *          0010 0101 or 1001.
 * https://oeis.org/A164493
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 40, u64 n <= 81, u128 n <= 163,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Automaton: Number of binary strings of length n avoiding
   the forbidden substrings 0010, 0101, and 1001.
   The function is O(n) and works for any seq_t and idx_t
   types provided by the build environment. */

#include <string.h>

seq_t a(idx_t n)
{
    /* For n less than the offset the sequence is undefined – return 0. */
    if (n < 4)
        return (seq_t)0;

    /* States are the prefixes of the forbidden patterns of length ≤3. */
    enum { STATE_COUNT = 9 };
    /* 0: "", 1: "0", 2: "1", 3: "00", 4: "01", 5: "10",
       6: "001", 7: "010", 8: "100" */
    static int trans[STATE_COUNT][2];
    static int trans_initialized = 0;

    if (!trans_initialized) {
        /* List of state strings. */
        const char *states_str[STATE_COUNT] = {
            "", "0", "1", "00", "01", "10",
            "001", "010", "100"
        };

        /* Forbidden full patterns. */
        const char *forbidden[3] = {"0010", "0101", "1001"};

        for (int s = 0; s < STATE_COUNT; ++s) {
            size_t len_s = strlen(states_str[s]);
            for (int b = 0; b < 2; ++b) {
                /* Build new string = states_str[s] + bit */
                char new_str[5]; /* max length 4 + NUL */
                memcpy(new_str, states_str[s], len_s);
                new_str[len_s] = (char)('0' + b);
                new_str[len_s + 1] = '\0';
                size_t len_new = len_s + 1;

                /* Check if the last 4 bits form a forbidden pattern. */
                int forbid = 0;
                if (len_new >= 4) {
                    const char *last4 = new_str + len_new - 4;
                    for (int f = 0; f < 3; ++f) {
                        if (memcmp(last4, forbidden[f], 4) == 0) {
                            forbid = 1;
                            break;
                        }
                    }
                }
                if (forbid) {
                    trans[s][b] = -1;
                    continue;
                }

                /* Find the longest suffix that is a prefix of a forbidden word. */
                int ns = 0; /* default to empty state */
                size_t max_l = (len_new < 3) ? len_new : 3;
                for (size_t l = max_l; l >= 1; --l) {
                    const char *suffix = new_str + len_new - l;
                    int found = 0;
                    for (int k = 0; k < STATE_COUNT; ++k) {
                        if (strlen(states_str[k]) == l &&
                            memcmp(suffix, states_str[k], l) == 0) {
                            ns = k;
                            found = 1;
                            break;
                        }
                    }
                    if (found)
                        break;
                }
                trans[s][b] = ns;
            }
        }
        trans_initialized = 1;
    }

    /* DP arrays for current and next step. */
    seq_t dp_prev[STATE_COUNT];
    seq_t dp_cur[STATE_COUNT];

    /* Initialize: length 0 has only the empty string. */
    for (int i = 0; i < STATE_COUNT; ++i)
        dp_prev[i] = (seq_t)0;
    dp_prev[0] = (seq_t)1;

    /* Iterate over lengths 1..n. */
    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < STATE_COUNT; ++i)
            dp_cur[i] = (seq_t)0;
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp_prev[s];
            if (val == (seq_t)0)
                continue;
            for (int b = 0; b < 2; ++b) {
                int ns = trans[s][b];
                if (ns < 0)
                    continue;
                dp_cur[ns] += val;
            }
        }
        /* Move to next length. */
        for (int i = 0; i < STATE_COUNT; ++i)
            dp_prev[i] = dp_cur[i];
    }

    /* Sum all state counts to get total number of valid strings. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < STATE_COUNT; ++i)
        total += dp_prev[i];
    return total;
}

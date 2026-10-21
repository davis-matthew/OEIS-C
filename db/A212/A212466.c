/*
 * A212466  Number of 0..3 arrays of length n+5 with sum no more than 9 in any
 *          length 6 subsequence (=50% duty cycle).
 * https://oeis.org/A212466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 12, u64 n <= 30, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A212466: Number of 0..3 arrays of length n+5 with sum ≤ 9 in any
   length‑6 window (50% duty cycle).
   The algorithm uses dynamic programming on the last 5 digits.
   For each position we keep all 4^5=1024 possible 5‑tuples as a state,
   storing how many sequences lead to that state.
   Complexities: O(n · 4^5) time, O(4^5) memory.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define STATE_COUNT 1024
#define STATE_MASK  0x3FF   /* 10 bits */

/* tables describing transitions and their sums */
static int  sum_digits[STATE_COUNT];
static uint16_t next_state[STATE_COUNT][4];
static uint8_t  next_cnt[STATE_COUNT];
static bool tables_built = false;

static void build_tables(void)
{
    for (int s = 0; s < STATE_COUNT; ++s) {
        /* compute the sum of the five base‑4 digits */
        int sum = 0;
        int tmp = s;
        for (int k = 0; k < 5; ++k) {
            sum += tmp & 3;
            tmp >>= 2;
        }
        sum_digits[s] = sum;

        uint8_t cnt = 0;
        for (int d = 0; d < 4; ++d) {
            if (sum + d <= 9) {
                uint16_t ns = ((s << 2) & STATE_MASK) | (uint16_t)d;
                next_state[s][cnt++] = ns;
            }
        }
        next_cnt[s] = cnt;
    }
    tables_built = true;
}

seq_t a(idx_t n)
{
    /* length of the array is n + 5 */
    idx_t N = n + 5;
    if (N < 6) return (seq_t)0;          /* defined only for n ≥ 1 */

    if (!tables_built) build_tables();

    /* DP arrays: number of sequences arriving at state s */
    seq_t curr[STATE_COUNT];
    seq_t next[STATE_COUNT];

    /* initialise: all 5‑tuples are admissible before the first window */
    for (int s = 0; s < STATE_COUNT; ++s) curr[s] = (seq_t)1;

    /* process positions 6 … N */
    for (idx_t pos = 6; pos <= N; ++pos) {
        for (int s = 0; s < STATE_COUNT; ++s) next[s] = (seq_t)0;

        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = curr[s];
            if (val == 0) continue;
            uint8_t c = next_cnt[s];
            for (uint8_t i = 0; i < c; ++i) {
                uint16_t ns = next_state[s][i];
                next[ns] += val;
            }
        }
        /* copy next → curr */
        for (int s = 0; s < STATE_COUNT; ++s) curr[s] = next[s];
    }

    /* sum over all final states */
    seq_t total = (seq_t)0;
    for (int s = 0; s < STATE_COUNT; ++s) total += curr[s];
    return total;
}

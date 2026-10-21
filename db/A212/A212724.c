/*
 * A212724  Number of 0..2 arrays of length n+5 with sum less than 6 in any
 *          length 6 subsequence (=less than 50% duty cycle).
 * https://oeis.org/A212724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 0..2 arrays of length n+5 such that every
 *            contiguous subsequence of length 6 has sum ≤5
 * Range:     u8 none, u16 n <= 7, u32 n <= 20, u64 n <= 45, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define STATES 243
#define POWER_3_4 81

/* Sum of the five base‑3 digits of each state. */
static uint8_t digit_sum[STATES];

/* Compute digit_sum on first use. */
static void init_digit_sum(void)
{
    for (int st = 0; st < STATES; ++st) {
        int temp = st;
        uint8_t sum = 0;
        for (int k = 0; k < 5; ++k) {
            sum += temp % 3;
            temp /= 3;
        }
        digit_sum[st] = sum;
    }
}

seq_t a(idx_t n)
{
    /* Lazily initialise digit_sum. */
    static int initialized = 0;
    if (!initialized) {
        init_digit_sum();
        initialized = 1;
    }

    /* Length of the sequence to be counted. */
    idx_t L = n + 5;

    /* DP tables.  Two rows are enough. */
    static seq_t dp[2][STATES];
    seq_t *cur = dp[0];
    seq_t *nxt = dp[1];

    /* Initial state: empty sequence. */
    for (int i = 0; i < STATES; ++i) cur[i] = 0;
    cur[0] = 1;

    for (idx_t step = 0; step < L; ++step) {
        for (int i = 0; i < STATES; ++i) nxt[i] = 0;
        for (int st = 0; st < STATES; ++st) {
            seq_t ways = cur[st];
            if (ways == 0) continue;
            for (int x = 0; x <= 2; ++x) {
                if (step >= 5 && (digit_sum[st] + x) > 5) continue;
                int new_st = ((st % POWER_3_4) * 3) + x;
                nxt[new_st] += ways;
            }
        }
        /* Swap current and next rows. */
        seq_t *tmp = cur; cur = nxt; nxt = tmp;
    }

    /* Sum all states: all sequences of length L satisfy the condition. */
    seq_t answer = 0;
    for (int i = 0; i < STATES; ++i) answer += cur[i];
    return answer;
}

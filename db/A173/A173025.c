/*
 * A173025  Numbers whose binary representation contains no isolated digits
 *          "11".
 * https://oeis.org/A173025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 136, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A173025 – Numbers whose binary representation contains no isolated “11”.
    a(1)=0; for n≥2, a(n) is the (n‑2)th positive integer that avoids a run
    of exactly two consecutive 1's.
    Implemented by combinatorial ranking using a small DP. */

#include <stdbool.h>
#include <stdint.h>

#define MAX_BITS 64   /* sufficient for n up to 10,000 */

static seq_t dp[MAX_BITS + 1][4];
static seq_t count_len[MAX_BITS + 1];
static bool initialized = false;

/* DP: dp[rem][run] = number of ways to fill rem bits given current run.
   run: 0 = last bit 0, 1 = last bit 1 & run length 1,
        2 = run length 2, 3 = run length ≥3.
   The DP respects the rule that a run of length 2 cannot end. */
static void init(void) {
    if (initialized) return;
    initialized = true;

    /* base case: no bits remaining */
    dp[0][0] = (seq_t)1;
    dp[0][1] = dp[0][3] = (seq_t)1;
    dp[0][2] = (seq_t)0;   /* ending with run length 2 is invalid */

    /* fill DP table for up to MAX_BITS bits */
    for (int rem = 1; rem <= MAX_BITS; ++rem) {
        for (int run = 0; run <= 3; ++run) {
            seq_t total = (seq_t)0;

            /* place 0: allowed only if current run != 2 */
            if (run != 2) {
                total += dp[rem - 1][0];
            }

            /* place 1: update run length */
            int next_run;
            if (run == 0)      next_run = 1;
            else if (run == 1) next_run = 2;
            else               next_run = 3;   /* run 2 or 3 → 3 */

            total += dp[rem - 1][next_run];
            dp[rem][run] = total;
        }
    }

    /* count_len[L] = number of positive integers of exact bit length L */
    for (int L = 1; L <= MAX_BITS; ++L) {
        count_len[L] = dp[L - 1][1];   /* leading 1 gives run=1 */
    }
}

/* Return a(n) for n ≥ 1 */
seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;

    init();

    /* zero-based index among positive valid numbers */
    seq_t rank0 = (seq_t)(n - 2);

    /* find the bit length L of the desired number */
    seq_t cumulative = (seq_t)0;
    int L;
    for (L = 1; L <= MAX_BITS; ++L) {
        seq_t cnt = count_len[L];
        if (cumulative + cnt > rank0) break;
        cumulative += cnt;
    }

    /* offset within numbers of length L */
    seq_t k = rank0 - cumulative;   /* 0‑based */

    /* build the number bit by bit */
    seq_t val = (seq_t)1;   /* leading 1 */
    int run = 1;            /* current run length after leading 1 */
    int rem = L - 1;        /* bits remaining to set */

    while (rem > 0) {
        seq_t count0 = (run != 2) ? dp[rem - 1][0] : (seq_t)0;

        if (k < count0) {
            /* choose bit 0 */
            val = (val << 1);
            run = 0;
        } else {
            /* choose bit 1 */
            k -= count0;
            val = (val << 1) | (seq_t)1;
            if (run == 0)          run = 1;
            else if (run == 1)     run = 2;
            else                   run = 3;   /* run 2 or 3 → 3 */
        }
        --rem;
    }

    return val;
}

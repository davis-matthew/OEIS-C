/*
 * A207496  Number of nX4 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence is computed by dynamic programming over the 9 valid
 *            4‑bit rows that avoid the horizontal forbidden patterns. For
 *            each pair of consecutive rows we keep a count, and a new row can
 *            be appended if for every column the vertical triple does not
 *            equal 001 or 100. The total number of n×4 arr...
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>


static const uint8_t valid_rows[9] = {2,4,5,9,10,12,13,14,15};   /* 0010,0100,0101,1001,1010,1100,1101,1110,1111 */

/* Transition table: for each state (prev2,prev1) give list of compatible new rows */
static int trans_count[81];
static int trans[81][9];
static int initialized = 0;

/* Cached answers and DP state */
static seq_t *answers = NULL;
static idx_t ans_cap = 0;
static idx_t max_ans_computed = 0;
static seq_t dp[81];
static seq_t newdp[81];

static void init(void) {
    if (initialized) return;
    /* Build transition table */
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            int st = i*9 + j;
            int cnt = 0;
            for (int k = 0; k < 9; ++k) {
                int ok = 1;
                for (int b = 0; b < 4; ++b) {
                    int p = (valid_rows[i] >> b) & 1;
                    int q = (valid_rows[j] >> b) & 1;
                    int r = (valid_rows[k] >> b) & 1;
                    if ((p == 0 && q == 0 && r == 1) ||
                        (p == 1 && q == 0 && r == 0)) {
                        ok = 0;
                        break;
                    }
                }
                if (ok) trans[st][cnt++] = k;
            }
            trans_count[st] = cnt;
        }
    }

    /* Allocate answer cache */
    ans_cap = 3;                /* indices 0..2 used initially */
    answers = (seq_t*)calloc(ans_cap, sizeof(seq_t));
    answers[1] = 9;             /* a(1) */
    answers[2] = 81;            /* a(2) */

    /* Initialize DP state for length 2: all pairs possible */
    for (int st = 0; st < 81; ++st) dp[st] = 1;

    max_ans_computed = 2;
    initialized = 1;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;        /* undefined for n < 1 */
    init();

    if (n <= max_ans_computed) return answers[n];

    /* Ensure answer cache large enough */
    if (n >= ans_cap) {
        size_t newcap = ans_cap ? ans_cap : 1;
        while (newcap <= n) newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(answers, newcap * sizeof(seq_t));
        if (!tmp) return 0;     /* memory allocation failed */
        answers = tmp;
        ans_cap = newcap;
    }

    for (idx_t len = max_ans_computed + 1; len <= n; ++len) {
        /* Reset newdp */
        for (int i = 0; i < 81; ++i) newdp[i] = 0;

        /* Transition from dp to newdp */
        for (int st = 0; st < 81; ++st) {
            seq_t cnt = dp[st];
            if (cnt == 0) continue;
            int i = st / 9;
            int j = st % 9;
            for (int t = 0; t < trans_count[st]; ++t) {
                int k = trans[st][t];
                int new_st = j*9 + k;
                newdp[new_st] += cnt;
            }
        }

        /* Compute answer for current length and copy newdp to dp */
        seq_t sum = 0;
        for (int st = 0; st < 81; ++st) {
            dp[st] = newdp[st];
            sum += newdp[st];
        }
        answers[len] = sum;
    }

    max_ans_computed = n;
    return answers[n];
}

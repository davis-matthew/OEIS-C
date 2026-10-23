/*
 * A181037  Number of nX3 binary matrices with no initial bit string in any
 *          row or column divisible by 6.
 * https://oeis.org/A181037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A181037
   Formula: a(n) = number of n×3 binary matrices where in every row and column
   no initial (prefix) binary string is divisible by 6.  The first row is forced
   to 111 and subsequent rows are counted by a dynamic program on column residues
   modulo 6.  */

/* The function is required by the build system. */
#include <stdint.h>

#define NUM_STATES 125   /* 5 residues (1..5) for each of 3 columns: 5^3 */

static inline int state_index(int r1, int r2, int r3)
{
    /* r1,r2,r3 are in 1..5 because residue 0 is forbidden */
    return (r1 - 1) * 25 + (r2 - 1) * 5 + (r3 - 1);
}

static inline void decode_state(int idx, int *r1, int *r2, int *r3)
{
    *r1 = idx / 25 + 1;
    *r2 = (idx / 5) % 5 + 1;
    *r3 = idx % 5 + 1;
}

seq_t a(idx_t n)
{
    /* special case for n = 1 (only the all‑ones row is allowed) */
    if (n == 1)
        return 1;

    /* ------- Pre‑compute all valid 3‑bit rows (row prefix condition) ------- */
    int valid_bits[8];
    int vb_b1[8];
    int vb_b2[8];
    int vb_b3[8];
    int valid_cnt = 0;

    for (int bits = 0; bits < 8; ++bits) {
        int b1 = (bits >> 2) & 1;
        int b2 = (bits >> 1) & 1;
        int b3 = bits & 1;

        int r1 = b1;
        if (r1 == 0) continue;

        int r2 = (b1 * 2 + b2) % 6;
        if (r2 == 0) continue;

        int r3 = (r2 * 2 + b3) % 6;
        if (r3 == 0) continue;

        valid_bits[valid_cnt] = bits;
        vb_b1[valid_cnt] = b1;
        vb_b2[valid_cnt] = b2;
        vb_b3[valid_cnt] = b3;
        ++valid_cnt;
    }

    /* ------- Dynamic programming over rows 2..n ------- */
    seq_t dp[NUM_STATES];
    seq_t next[NUM_STATES];
    for (int i = 0; i < NUM_STATES; ++i) {
        dp[i] = 0;
        next[i] = 0;
    }

    /* initial state after the forced first row (111) */
    int init_idx = state_index(1, 1, 1);   /* residues are all 1 after reading '1' */
    dp[init_idx] = 1;

    /* iterate over the remaining n-1 rows */
    for (idx_t step = 1; step < n; ++step) {
        /* reset next array */
        for (int i = 0; i < NUM_STATES; ++i)
            next[i] = 0;

        for (int s = 0; s < NUM_STATES; ++s) {
            if (dp[s] == 0) continue;
            int r1, r2, r3;
            decode_state(s, &r1, &r2, &r3);

            for (int t = 0; t < valid_cnt; ++t) {
                int t1 = (r1 * 2 + vb_b1[t]) % 6;
                int t2 = (r2 * 2 + vb_b2[t]) % 6;
                int t3 = (r3 * 2 + vb_b3[t]) % 6;

                if (t1 == 0 || t2 == 0 || t3 == 0) continue;  /* forbidden prefix */
                int dest = state_index(t1, t2, t3);
                next[dest] += dp[s];
            }
        }
        /* swap dp and next */
        for (int i = 0; i < NUM_STATES; ++i) {
            dp[i] = next[i];
        }
    }

    /* sum all states to get the total number of matrices */
    seq_t total = 0;
    for (int i = 0; i < NUM_STATES; ++i)
        total += dp[i];

    return total;
}

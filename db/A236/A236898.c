/*
 * A236898  Number of (n+1)X(4+1) 0..2 arrays with the maximum plus the upper
 *          median of every 2X2 subblock equal.
 * https://oeis.org/A236898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n) for OEIS sequence A236898.
    The value a(n) is the number of (n+1)×5 arrays with entries in {0,1,2}
    such that, for every 2×2 subblock, the sum of its maximum value and its
    upper median is the same across the whole array.
    This is computed by dynamic programming over rows.
    The state space is all 5‑tuples of {0,1,2} (3^5 = 243 states).
    For each possible common sum s∈{0,1,2,3,4} we pre‑compute a directed
    graph whose edges (i→j) correspond to consecutive rows i and j whose
    2×2 blocks all have that sum s.  For fixed s the number of arrays of
    length n+1 rows is the number of length‑n walks in this graph.
    The final result is the sum of these counts over all s. */

#include <stddef.h>   /* for size_t */

#define NUM_STATES 243
#define NUM_COLS   5
#define NUM_SUMS   5   /* possible sums 0..4 */

static int state_digits[NUM_STATES][NUM_COLS];
static int block_sum[81];                     /* 3^4 = 81 block patterns */
static unsigned short next_dst[NUM_SUMS][NUM_STATES][243];
static unsigned char  next_cnt[NUM_SUMS][NUM_STATES];
static int          precomp_done = 0;

/* Decode a state index to its base‑3 digits. */
static void
compute_state_digits(void)
{
    for (int idx = 0; idx < NUM_STATES; ++idx) {
        int x = idx;
        for (int pos = NUM_COLS - 1; pos >= 0; --pos) {
            state_digits[idx][pos] = x % 3;
            x /= 3;
        }
    }
}

/* Compute block_sum for all 4‑digit patterns. */
static void
compute_block_sum(void)
{
    for (int a00 = 0; a00 <= 2; ++a00)
        for (int a01 = 0; a01 <= 2; ++a01)
            for (int a10 = 0; a10 <= 2; ++a10)
                for (int a11 = 0; a11 <= 2; ++a11) {
                    int arr[4] = { a00, a01, a10, a11 };
                    /* simple sorting of 4 values */
                    for (int p = 0; p < 4; ++p)
                        for (int q = p + 1; q < 4; ++q)
                            if (arr[p] > arr[q]) {
                                int t = arr[p];
                                arr[p] = arr[q];
                                arr[q] = t;
                            }
                    int maxv   = arr[3];
                    int med3   = arr[2];            /* upper median (third largest) */
                    int sumval = maxv + med3;
                    int key = (((a00 * 3 + a01) * 3 + a10) * 3 + a11);
                    block_sum[key] = sumval;
                }
}

/* Precompute adjacency lists for each possible common sum s. */
static void
compute_next(void)
{
    for (int s = 0; s < NUM_SUMS; ++s) {
        for (int i = 0; i < NUM_STATES; ++i)
            next_cnt[s][i] = 0;

        for (int i = 0; i < NUM_STATES; ++i) {
            for (int j = 0; j < NUM_STATES; ++j) {
                int ok = 1;
                for (int c = 0; c < NUM_COLS - 1; ++c) {
                    int d0 = state_digits[i][c];
                    int d1 = state_digits[i][c + 1];
                    int d2 = state_digits[j][c];
                    int d3 = state_digits[j][c + 1];
                    int key = (((d0 * 3 + d1) * 3 + d2) * 3 + d3);
                    if (block_sum[key] != s) {
                        ok = 0;
                        break;
                    }
                }
                if (ok) {
                    unsigned char cnt = next_cnt[s][i];
                    next_dst[s][i][cnt] = (unsigned short)j;
                    next_cnt[s][i] = cnt + 1;
                }
            }
        }
    }
}

/* One‑time precomputation. */
static void
precompute(void)
{
    if (precomp_done) return;
    compute_state_digits();
    compute_block_sum();
    compute_next();
    precomp_done = 1;
}

/* Main function required by the build system. */
seq_t a(idx_t n)
{
    precompute();

    seq_t total = 0;

    for (int s = 0; s < NUM_SUMS; ++s) {
        seq_t counts[NUM_STATES];
        seq_t new_counts[NUM_STATES];
        /* initial row can be any state */
        for (int i = 0; i < NUM_STATES; ++i) counts[i] = (seq_t)1;

        for (idx_t step = 0; step < n; ++step) {
            /* zero new_counts */
            for (int i = 0; i < NUM_STATES; ++i) new_counts[i] = 0;

            for (int i = 0; i < NUM_STATES; ++i) {
                seq_t c = counts[i];
                if (c == 0) continue;
                unsigned char limit = next_cnt[s][i];
                for (unsigned char k = 0; k < limit; ++k) {
                    unsigned short j = next_dst[s][i][k];
                    new_counts[j] += c;
                }
            }

            /* copy new_counts into counts for next step */
            for (int i = 0; i < NUM_STATES; ++i) counts[i] = new_counts[i];
        }

        /* sum all walks for this sum s */
        seq_t sum_s = 0;
        for (int i = 0; i < NUM_STATES; ++i) sum_s += counts[i];
        total += sum_s;
    }

    return total;
}

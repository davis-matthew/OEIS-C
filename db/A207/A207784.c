/*
 * A207784  Number of nX7 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 0 and 1 0 1 vertically.
 * https://oeis.org/A207784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 binary arrays avoiding 0 0 0 and 0 1 1
 *            horizontally and 0 0 0 and 1 0 1 vertically. Computed by dynamic
 *            programming over pairs of rows
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* ---------- parameters ---------- */
#define MAX_N 10000                    /* maximal n expected */
#define MAX_COLS 7
#define MAX_ROW_BITS 128              /* 2^7 */
#define MAX_STATES (MAX_ROW_BITS*MAX_ROW_BITS) /* 16384 */

/* ---------- static storage ---------- */
static int        row_bits[MAX_ROW_BITS];      /* bitmask of each valid row */
static int        valid_ids[MAX_ROW_BITS];     /* indices of valid rows in row_bits */
static int        M = 0;                       /* count of valid rows */

static unsigned char  trans_len[MAX_STATES];              /* how many next rows for each state */
static int            trans_rows[MAX_STATES][MAX_ROW_BITS];/* list of next row indices */

static seq_t  dp_curr[MAX_STATES];
static seq_t  dp_next[MAX_STATES];
static seq_t  answers[MAX_N+1];

static idx_t  current_len = 0;   /* length of sequences currently stored in dp_curr */
static int    initialized = 0;

/* ---------- helper: check if a 7-bit pattern is valid for a row ---------- */
static int is_valid_row(int bits)
{
    /* no 000 or 011 in any consecutive 3 bits */
    for (int j = 0; j <= MAX_COLS - 3; ++j) {
        int sub = (bits >> j) & 0x7;            /* 3 bits */
        if (sub == 0 || sub == 0x3)             /* 000 or 011 */
            return 0;
    }
    return 1;
}

/* ---------- helper: check vertical constraints for triple (a,b,c) ---------- */
static int vertical_ok(int a, int b, int c)
{
    /* disallow 000 and 101 */
    int pattern = (a << 2) | (b << 1) | c;
    return pattern != 0 && pattern != 5;
}

/* ---------- initialization: build valid rows and transitions ---------- */
static void prepare()
{
    if (initialized) return;
    /* build list of valid rows */
    for (int b = 0; b < (1 << MAX_COLS); ++b) {
        if (is_valid_row(b)) {
            row_bits[M] = b;
            valid_ids[M] = M;
            ++M;
        }
    }
    /* precompute transitions */
    int S = M * M;
    for (int s = 0; s < S; ++s) {
        int i = s / M;
        int j = s % M;
        int r1 = row_bits[i];
        int r2 = row_bits[j];
        int cnt = 0;
        for (int k = 0; k < M; ++k) {
            int r3 = row_bits[k];
            int ok = 1;
            for (int col = 0; col < MAX_COLS; ++col) {
                int a = (r1 >> col) & 1;
                int b = (r2 >> col) & 1;
                int c = (r3 >> col) & 1;
                if (!vertical_ok(a, b, c)) { ok = 0; break; }
            }
            if (ok) {
                trans_rows[s][cnt++] = k;
            }
        }
        trans_len[s] = cnt;
    }

    /* base cases */
    if (M > 0) {
        answers[1] = (seq_t)M;
        answers[2] = (seq_t)M * M;
    }
    /* initialize dp for length 2: all pairs count as 1 */
    int S2 = M * M;
    for (int s = 0; s < S2; ++s) dp_curr[s] = (seq_t)1;
    current_len = 2;
    initialized = 1;
}

/* ---------- compute up to n ---------- */
static void compute_up_to(idx_t n)
{
    if (!initialized) prepare();
    if (n <= current_len) return;          /* already computed */

    int S = M * M;
    for (idx_t len = current_len; len < n; ++len) {
        /* compute next layer */
        /* zero dp_next */
        memset(dp_next, 0, S * sizeof(seq_t));
        for (int s = 0; s < S; ++s) {
            seq_t val = dp_curr[s];
            if (val == 0) continue;
            int j = s % M;               /* second row of the state */
            int c = trans_len[s];
            for (int t = 0; t < c; ++t) {
                int k = trans_rows[s][t];
                int next_idx = j * M + k;
                dp_next[next_idx] += val;
            }
        }
        /* swap dp_curr and dp_next by copying */
        memcpy(dp_curr, dp_next, S * sizeof(seq_t));

        /* sum all states for current length */
        seq_t sum = 0;
        for (int s = 0; s < S; ++s) sum += dp_curr[s];
        answers[len + 1] = sum;
    }
    current_len = n;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, undefined below */
    compute_up_to(n);
    return answers[n];
}

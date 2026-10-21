/*
 * A224053  Number of n X 4 0..2 arrays with rows and columns unimodal and
 *          antidiagonals nondecreasing.
 * https://oeis.org/A224053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 15, u64 n <= 75, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A224053: number of n×4 arrays (entries 0..2) whose rows and columns
 *  are unimodal and whose antidiagonals are nondecreasing.
 *
 *  The implementation uses dynamic programming over column states.
 *  A state consists of, for each of the four columns, the last value
 *  (0..2) and a flag that indicates whether a decrease has already
 *  occurred in that column.  Each state needs 3 bits, 12 bits total
 *  (4096 states).  The DP iterates over rows; a transition is allowed
 *  only if the new row is unimodal, the column monotonicity is respected,
 *  and the antidiagonal constraint (prev column ≤ next column) holds.
 *
 *  The first row is unrestricted except for row unimodality, so we
 *  initialise the DP with all admissible rows (46 of them).  The
 *  transition table is pre‑computed once.  The function is fully
 *  self‑contained and works with any seq_t / idx_t type supplied by
 *  the build.  No external libraries are required.  The algorithm
 *  runs in O(n·#states·avgSucc) which is well below the time limit
 *  for n up to 10000.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Number of all column states (3 values × 2 flags ^ 4 columns) */
#define TOT_STATES 4096
/* Upper bound on number of successors per state; safe margin */
#define MAX_SUCC   70

/* Cached data, built on the first call */
static uint8_t row_patterns[81][4];   /* all unimodal 4‑tuples (max 46) */
static int      total_patterns;      /* number of patterns actually stored */

static uint16_t succ[TOT_STATES][MAX_SUCC]; /* successor indices */
static int      succ_cnt[TOT_STATES];       /* number of successors of a state */
static bool     built = false;

/* ------------------------------------------------------------------ */
/* Helper: check whether a 4‑tuple is unimodal. */
static bool is_unimodal(const uint8_t v[4]) {
    for (int peak = 0; peak < 4; ++peak) {
        bool increasing = true;
        for (int i = 0; i < peak; ++i)
            if (v[i] > v[i+1]) { increasing = false; break; }
        if (!increasing) continue;
        for (int i = peak; i < 3; ++i)
            if (v[i] < v[i+1]) { increasing = false; break; }
        if (increasing) return true;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Build admissible rows and the transition table.  This is done once. */
static void build(void) {
    /* --- Generate all 4‑tuples over {0,1,2} and keep the unimodal ones. */
    total_patterns = 0;
    for (int a = 0; a <= 2; ++a)
        for (int b = 0; b <= 2; ++b)
            for (int c = 0; c <= 2; ++c)
                for (int d = 0; d <= 2; ++d) {
                    uint8_t v[4] = { (uint8_t)a, (uint8_t)b, (uint8_t)c, (uint8_t)d };
                    if (is_unimodal(v)) {
                        row_patterns[total_patterns][0] = v[0];
                        row_patterns[total_patterns][1] = v[1];
                        row_patterns[total_patterns][2] = v[2];
                        row_patterns[total_patterns][3] = v[3];
                        ++total_patterns;
                    }
                }

    /* --- Precompute all transitions. */
    for (int s = 0; s < TOT_STATES; ++s) {
        /* Decode the current state: for each column j, store last value and flag. */
        uint8_t val[4];
        uint8_t flag[4];
        for (int j = 0; j < 4; ++j) {
            int bits = (s >> (3*j)) & 0x7;         /* 3 bits: flag (bit 2) + value (bits 0-1) */
            flag[j] = (bits >> 2) & 1;
            val[j]  = bits & 0x3;
        }

        int k = 0; /* successor counter */
        for (int r = 0; r < total_patterns; ++r) {
            bool ok = true;
            /* Antidiagonal constraint: pattern[j-1] >= val[j] for j=1..3. */
            for (int j = 1; j < 4; ++j) {
                if (row_patterns[r][j-1] < val[j]) { ok = false; break; }
            }
            if (!ok) continue;

            /* Column monotonicity and flag update. */
            uint8_t newFlag[4];
            for (int j = 0; j < 4; ++j) {
                uint8_t prev = val[j];
                uint8_t cur  = row_patterns[r][j];
                if (prev < cur) {
                    if (flag[j]) { ok = false; break; }   /* increase after a decrease */
                    newFlag[j] = flag[j];
                } else if (prev > cur) {
                    newFlag[j] = 1;                       /* once decreasing stays decreasing */
                } else {
                    newFlag[j] = flag[j];
                }
            }
            if (!ok) continue;

            /* Encode the new state. */
            int ns = 0;
            for (int j = 0; j < 4; ++j) {
                int bits = (newFlag[j] << 2) | (row_patterns[r][j] & 0x3);
                ns |= bits << (3*j);
            }
            succ[s][k++] = (uint16_t)ns;
        }
        succ_cnt[s] = k;
    }

    built = true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function. */
seq_t a(idx_t n) {
    if (!built) build();
    if (n == 0) return (seq_t)0;   /* out of offset, safe to return 0 */

    static seq_t dp[TOT_STATES];
    static seq_t ndp[TOT_STATES];

    /* Zero initialise the dp array for the first row. */
    for (int i = 0; i < TOT_STATES; ++i) dp[i] = (seq_t)0;

    /* Set up dp for the first row (row patterns only). */
    seq_t one = (seq_t)1;
    for (int r = 0; r < total_patterns; ++r) {
        int code = 0;
        for (int j = 0; j < 4; ++j) {
            int bits = row_patterns[r][j] & 0x3;   /* flag = 0 */
            code |= bits << (3*j);
        }
        dp[code] += one;
    }

    /* All subsequent rows */
    size_t steps = (size_t)(n - 1);     /* remaining rows after the first */
    for (size_t step = 0; step < steps; ++step) {
        for (int i = 0; i < TOT_STATES; ++i) ndp[i] = (seq_t)0;
        for (int s = 0; s < TOT_STATES; ++s) {
            seq_t cnt = dp[s];
            if (cnt == (seq_t)0) continue;
            int k = succ_cnt[s];
            for (int t = 0; t < k; ++t) {
                uint16_t ns = succ[s][t];
                ndp[ns] += cnt;
            }
        }
        /* swap dp and ndp */
        for (int i = 0; i < TOT_STATES; ++i) dp[i] = ndp[i];
    }

    /* Sum all states to get the total count. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < TOT_STATES; ++i) total += dp[i];
    return total;
}

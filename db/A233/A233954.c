/*
 * A233954  Number of (n+1)X(4+1) 0..5 arrays with every 2X2 subblock having
 *          the sum of the squares of the edge differences equal to 10, and no
 *          two adjacent values equal.
 * https://oeis.org/A233954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed via dynamic programming over the 3750 possible rows. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Row representation */
#define ROW_DIGITS 5
#define MAX_DIGIT 5
#define STATES (6ULL * 5ULL * 5ULL * 5ULL * 5ULL)   /* 3750 */

/* Structure for adjacency list */
typedef struct {
    int *data;
    size_t size;
} neighlist_t;

/* Global static data */
static int rows[STATES][ROW_DIGITS];
static neighlist_t *neighbors = NULL;
static size_t num_states = STATES;

/* Helper to test whether two rows can be adjacent */
static bool allowed_pair(size_t i, size_t j)
{
    int const *ri = rows[i];
    int const *rj = rows[j];

    /* vertical adjacency must not be equal */
    for (int k = 0; k < ROW_DIGITS; ++k)
        if (ri[k] == rj[k])
            return false;

    /* 2×2 blocks constraint */
    for (int c = 0; c < ROW_DIGITS - 1; ++c) {
        int a = ri[c];
        int b = ri[c + 1];
        int c_ = rj[c];
        int d = rj[c + 1];
        int diff = (a - b);
        int sum = diff * diff;
        diff = a - c_;
        sum += diff * diff;
        diff = b - d;
        sum += diff * diff;
        diff = c_ - d;
        sum += diff * diff;
        if (sum != 10)
            return false;
    }
    return true;
}

/* Build all rows and adjacency lists (called first time) */
static void build(void)
{
    /* Generate all valid rows (no equal horizontal neighbours) */
    size_t idx = 0;
    for (int a = 0; a <= MAX_DIGIT; ++a)
        for (int b = 0; b <= MAX_DIGIT; ++b) if (b != a)
            for (int c = 0; c <= MAX_DIGIT; ++c) if (c != b)
                for (int d = 0; d <= MAX_DIGIT; ++d) if (d != c)
                    for (int e = 0; e <= MAX_DIGIT; ++e) if (e != d) {
                        rows[idx][0] = a;
                        rows[idx][1] = b;
                        rows[idx][2] = c;
                        rows[idx][3] = d;
                        rows[idx][4] = e;
                        ++idx;
                    }
    /* Number of rows should be STATES */
    (void)idx; /* silence unused variable warning */

    /* First pass: count degrees */
    size_t *deg = (size_t *)calloc(num_states, sizeof(size_t));
    for (size_t i = 0; i < num_states; ++i) {
        for (size_t j = i + 1; j < num_states; ++j) {
            if (allowed_pair(i, j)) {
                ++deg[i];
                ++deg[j];
            }
        }
    }

    /* Allocate adjacency lists */
    neighbors = (neighlist_t *)malloc(num_states * sizeof(neighlist_t));
    for (size_t i = 0; i < num_states; ++i) {
        neighbors[i].size = 0;
        neighbors[i].data = (deg[i] > 0) ? (int *)malloc(deg[i] * sizeof(int)) : NULL;
    }

    /* Second pass: fill adjacency lists */
    for (size_t i = 0; i < num_states; ++i) {
        for (size_t j = i + 1; j < num_states; ++j) {
            if (allowed_pair(i, j)) {
                neighbors[i].data[neighbors[i].size++] = (int)j;
                neighbors[j].data[neighbors[j].size++] = (int)i;
            }
        }
    }

    free(deg);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Build once */
    static bool built = false;
    if (!built) {
        build();
        built = true;
    }

    /* Number of rows */
    idx_t rows_cnt = n + 1;

    /* DP arrays */
    static seq_t *dp0 = NULL;
    static seq_t *dp1 = NULL;
    static size_t dp_size = 0;
    if (dp0 == NULL) {
        dp_size = num_states;
        dp0 = (seq_t *)malloc(dp_size * sizeof(seq_t));
        dp1 = (seq_t *)malloc(dp_size * sizeof(seq_t));
    }

    /* Initialise dp0 to 1 (each row can start) */
    for (size_t i = 0; i < dp_size; ++i)
        dp0[i] = (seq_t)1;

    /* DP over rows */
    for (idx_t step = 1; step < rows_cnt; ++step) {
        /* zero dp1 */
        for (size_t i = 0; i < dp_size; ++i)
            dp1[i] = (seq_t)0;
        /* transition */
        for (size_t i = 0; i < dp_size; ++i) {
            seq_t val = dp0[i];
            if (val == 0)
                continue;
            const int *neigh = neighbors[i].data;
            size_t deg = neighbors[i].size;
            for (size_t t = 0; t < deg; ++t) {
                int j = neigh[t];
                dp1[j] += val;
            }
        }
        /* swap dp0 and dp1 */
        seq_t *tmp = dp0;
        dp0 = dp1;
        dp1 = tmp;
    }

    /* Sum all states to get total number of arrays */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < dp_size; ++i)
        result += dp0[i];

    return result;
}

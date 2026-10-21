/*
 * A224386  Number of nX3 0..3 arrays with diagonals and antidiagonals
 *          unimodal and rows nondecreasing.
 * https://oeis.org/A224386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 arrays with entries 0..3, rows
 *            nondecreasing, and all length‑3 main and anti‑diagonal triples
 *            unimodal. Recurrence is implemented by dynamic programming over
 *            rows
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Row of length 3 with 0 <= a <= b <= c <= 3. */
static struct Row {
    unsigned a, b, c;
} rows[20];

/* Transition table: trans[p][c][n] is true iff rows[p], rows[c], rows[n]
   satisfy the two diagonal unimodality constraints. */
static bool trans[20][20][20];
static bool init_done = false;

/* Helper to fill the rows array and the transition table. */
static void init(void) {
    if (init_done) return;
    /* Enumerate all weakly increasing triples in 0..3. */
    int k = 0;
    for (int a = 0; a <= 3; ++a)
        for (int b = a; b <= 3; ++b)
            for (int c = b; c <= 3; ++c) {
                rows[k].a = a;
                rows[k].b = b;
                rows[k].c = c;
                ++k;
            }
    /* Precompute transitions.  A length‑3 triple is unimodal iff it is
       not of the form x > y < z.  */
    for (int p = 0; p < 20; ++p)
        for (int c = 0; c < 20; ++c)
            for (int n = 0; n < 20; ++n) {
                bool ok1 = !(rows[p].a > rows[c].b && rows[c].b < rows[n].c);
                bool ok2 = !(rows[p].c > rows[c].b && rows[c].b < rows[n].a);
                trans[p][c][n] = ok1 && ok2;
            }
    init_done = true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* below offset, safe to return 0 */
    init();

    if (n == 1) return (seq_t)20;
    if (n == 2) {
        seq_t val = (seq_t)20;
        return val * val;
    }

    /* DP over rows: dp[parity][prev][curr] stores counts of prefixes
       ending with rows prev and curr. */
    seq_t dp[2][20][20];
    /* Initialize for the first two rows: any pair is allowed. */
    for (int i = 0; i < 20; ++i)
        for (int j = 0; j < 20; ++j)
            dp[0][i][j] = (seq_t)1;

    /* Iterate over remaining rows. */
    for (idx_t pos = 3; pos <= n; ++pos) {
        int cur = pos & 1;          /* current parity */
        int prev = (pos - 1) & 1;   /* previous parity */

        /* Clear current layer. */
        for (int i = 0; i < 20; ++i)
            for (int j = 0; j < 20; ++j)
                dp[cur][i][j] = (seq_t)0;

        /* Transition from previous layer to current. */
        for (int pi = 0; pi < 20; ++pi) {
            for (int ci = 0; ci < 20; ++ci) {
                seq_t val = dp[prev][pi][ci];
                if (val == 0) continue;
                for (int ni = 0; ni < 20; ++ni) {
                    if (trans[pi][ci][ni])
                        dp[cur][ci][ni] += val;
                }
            }
        }
    }

    /* Sum all states of the final parity. */
    int final = (n & 1) ? 1 : 0;
    seq_t result = 0;
    for (int i = 0; i < 20; ++i)
        for (int j = 0; j < 20; ++j)
            result += dp[final][i][j];

    return result;
}

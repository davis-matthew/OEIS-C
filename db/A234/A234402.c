/*
 * A234402  Number of (n+1)X(3+1) 0..6 arrays with every 2X2 subblock having
 *          the sum of the absolute values of the edge differences equal to
 *          12.
 * https://oeis.org/A234402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 arrays with entries 0..6 such that
 *            every 2×2 subblock has the sum of the absolute values of its
 *            four edge differences equal to 12. Computed by DP over the 2401
 *            possible row states
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 8, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define S 2401          /* 7^4 possible rows */
#define MAX_N 10000     /* safety limit for cached values */

static unsigned char state_digits[S][4];   /* base‑7 digits of each state */
static int *compat[S];                    /* adjacency lists (prev → next)  */
static int compat_count[S];              /* number of neighbours for each state */

static seq_t dp[2][S];                    /* DP rows, alternating buffers   */
static seq_t a_cache[MAX_N+1];            /* cached a(n) values, 1‑based     */

static int cur_state_idx = 0;             /* index (0 or 1) of current dp    */
static idx_t cur_rows = 1;                /* number of rows already processed */
static idx_t next_n_to_compute = 1;       /* next n for which a(n) will be cached */
static bool initialized = false;

/* Check whether two states s → t are compatible.  The four digits of
   a row are accessed from state_digits.  For each of the three
   adjacent column pairs we need the sum of the four edge differences
   to be exactly 12.  */
static bool compatible(const int s, const int t)
{
    const unsigned char *ds = state_digits[s];
    const unsigned char *dt = state_digits[t];
    for (int j = 0; j < 3; ++j) {
        int a = ds[j];
        int b = ds[j+1];
        int c = dt[j];
        int d = dt[j+1];
        int sum = (a > b ? a-b : b-a)
                + (a > c ? a-c : c-a)
                + (b > d ? b-d : d-b)
                + (c > d ? c-d : d-c);
        if (sum != 12)
            return false;
    }
    return true;
}

/* Pre‑compute state digits and compatibility lists. */
static void init(void)
{
    /* Compose digits for all 2401 states.  The least significant digit
       corresponds to column 0. */
    for (int st = 0; st < S; ++st) {
        int v = st;
        for (int col = 0; col < 4; ++col) {
            state_digits[st][col] = (unsigned char)(v % 7);
            v /= 7;
        }
    }

    /* Count neighbours for each state (first pass). */
    for (int s = 0; s < S; ++s) {
        compat_count[s] = 0;
        for (int t = 0; t < S; ++t)
            if (compatible(s, t))
                ++compat_count[s];
    }

    /* Allocate adjacency lists. */
    for (int s = 0; s < S; ++s) {
        if (compat_count[s] > 0)
            compat[s] = (int *)malloc(compat_count[s] * sizeof(int));
        else
            compat[s] = NULL;
    }

    /* Fill adjacency lists (second pass). */
    int *place[S];
    for (int s = 0; s < S; ++s)
        place[s] = compat[s];
    for (int s = 0; s < S; ++s) {
        for (int t = 0; t < S; ++t) {
            if (compatible(s, t))
                *(place[s]++) = t;
        }
    }

    /* Initialise DP arrays with a single row (all states allowed). */
    for (int i = 0; i < S; ++i) {
        dp[0][i] = 1;
        dp[1][i] = 0;
    }
    cur_state_idx = 0;
    cur_rows = 1;           /* we have processed one row so far   */
    next_n_to_compute = 1;  /* a(1) will be computed after the next step */
    a_cache[0] = 0;         /* a(0) not used, but keep zero */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!initialized) {
        init();
        initialized = true;
    }

    if (n < 1)
        return 0; /* out of offset, behaviour unspecified */

    /* Extend the computation until a(n) is available. */
    while (next_n_to_compute <= n) {
        int cur = cur_state_idx;
        int nxt = 1 - cur;

        /* Zero next buffer. */
        for (int i = 0; i < S; ++i)
            dp[nxt][i] = 0;

        /* Transition step. */
        for (int s = 0; s < S; ++s) {
            seq_t val = dp[cur][s];
            if (val == (seq_t)0) continue;
            int *nbr = compat[s];
            int cnt = compat_count[s];
            for (int k = 0; k < cnt; ++k) {
                int t = nbr[k];
                dp[nxt][t] += val;
            }
        }

        cur_state_idx = nxt;
        ++cur_rows;          /* we have processed one more row now   */

        /* Sum current DP to obtain a(n) for this number of rows. */
        seq_t total = 0;
        for (int i = 0; i < S; ++i)
            total += dp[cur_state_idx][i];

        /* Store in cache.  The index aligns with the OEIS offset. */
        a_cache[next_n_to_compute] = total;
        ++next_n_to_compute;
    }

    return a_cache[n];
}

/*
 * A207350  Number of 6Xn 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 0 and 1 1 1 vertically.
 * https://oeis.org/A207350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 6×n binary matrices that avoid * horizontally
 *            000 and 011, * vertically 000 and 111. * The algorithm uses
 *            dynamic programming over the 26 allowed * column patterns and
 *            their pairwise transitions
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 16, u64 n <= 39, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* seq_t and idx_t are provided by the build system. */

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------ *
     *  All data structures are static so the function is safe for   *
     *  repeated calls and independent of the order of n.           *
     * ------------------------------------------------------------ */
    /*--- constants -------------------------------------------------*/
    enum {
        MAX_ROWS         = 6,
        MAX_COL_PATTERNS = 64,               /* 2^6 */
        MAX_ALLOWED      = 26,               /* computed */
        MAX_STATE        = MAX_ALLOWED * MAX_ALLOWED,
        MAX_N            = 10000              /* problem limit */
    };

    /*--- static data ----------------------------------------------*/
    static bool init_done = false;          /* flag after first init */
    static int  cols[MAX_ALLOWED];          /* allowed column patterns */
    static int  cols_cnt = 0;               /* actual number of columns */

    /* transition tables: from pair (i,j) to next pair (j,k) */
    static int  next_state[MAX_STATE][MAX_ALLOWED];
    static int  next_count[MAX_STATE];

    /* DP arrays for current/previous column pair counts   */
    static seq_t dp_prev[MAX_STATE];
    static seq_t dp_cur[MAX_STATE];

    /* memoisation cache for sequence values                 */
    static seq_t cache[MAX_N + 1];
    static bool   cached[MAX_N + 1];
    static idx_t  cache_max = 0;            /* largest n already cached */

    /* ------------------------------------------------------------ */
    /*                         initialisation                       */
    /* ------------------------------------------------------------ */
    if (!init_done) {
        /* 1. enumerate allowed column patterns (vertical constraints) */
        for (int p = 0; p < MAX_COL_PATTERNS; ++p) {
            bool ok = true;
            /* each run of length 3 inside the column is forbidden  */
            for (int r = 0; r <= MAX_ROWS - 3 && ok; ++r) {
                int b0 = (p >> r)     & 1;
                int b1 = (p >> (r+1)) & 1;
                int b2 = (p >> (r+2)) & 1;
                if ((b0 == 0 && b1 == 0 && b2 == 0) ||
                    (b0 == 1 && b1 == 1 && b2 == 1))
                    ok = false;
            }
            if (ok) {
                cols[cols_cnt++] = p;
            }
        }

        /* 2. build transition tables */
        for (int i = 0; i < cols_cnt; ++i) {
            for (int j = 0; j < cols_cnt; ++j) {
                int s = i * cols_cnt + j;      /* index of pair (i,j) */
                int c = 0;                     /* number of allowed k */
                for (int k = 0; k < cols_cnt; ++k) {
                    /* check horizontal constraint for all rows */
                    bool ok = true;
                    int p = cols[i], q = cols[j], r = cols[k];
                    for (int row = 0; row < MAX_ROWS && ok; ++row) {
                        int b1 = (p >> row) & 1;
                        int b2 = (q >> row) & 1;
                        int b3 = (r >> row) & 1;
                        if ((b1 == 0 && b2 == 0 && b3 == 0) ||
                            (b1 == 0 && b2 == 1 && b3 == 1))
                            ok = false;
                    }
                    if (ok) {
                        next_state[s][c++] = j * cols_cnt + k;
                    }
                }
                next_count[s] = c;
            }
        }

        /* 3. initialise DP for n = 2 (all pairs allowed) */
        for (int s = 0; s < MAX_STATE; ++s) {
            dp_prev[s] = 1;                 /* every pair is a start */
        }

        /* 4. compute and cache values for n = 1 and n = 2 */
        cache[0] = 0;                       /* unused */
        cache[1] = (seq_t)cols_cnt;         /* single column */
        cache[2] = (seq_t)cols_cnt * (seq_t)cols_cnt; /* two columns */
        cached[1] = true;
        cached[2] = true;
        cache_max = 2;

        init_done = true;
    }

    /* ------------------------------------------------------------ */
    /*                        compute up to n                       */
    /* ------------------------------------------------------------ */
    if (n <= cache_max && cached[n]) {
        return cache[n];
    }

    /* grow DP step by step from cached maximum to requested n */
    for (idx_t cur = cache_max + 1; cur <= n; ++cur) {
        /* reset dp_cur for accumulation */
        for (int s = 0; s < MAX_STATE; ++s) {
            dp_cur[s] = 0;
        }

        /* propagate each state to its allowed successors */
        for (int s = 0; s < MAX_STATE; ++s) {
            seq_t val = dp_prev[s];
            if (val == 0) continue;
            int cnt = next_count[s];
            int *ns  = next_state[s];
            for (int t = 0; t < cnt; ++t) {
                dp_cur[ ns[t] ] += val;
            }
        }

        /* sum over all states to get total number of arrays of length cur */
        seq_t total = 0;
        for (int s = 0; s < MAX_STATE; ++s) {
            total += dp_cur[s];
        }
        cache[cur] = total;
        cached[cur] = true;

        /* prepare for next iteration */
        for (int s = 0; s < MAX_STATE; ++s) {
            dp_prev[s] = dp_cur[s];
        }
    }
    cache_max = n;

    return cache[n];
}

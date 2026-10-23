/*
 * A008970  Triangle T(n,k) = P(n,k)/2, n >= 2, 1 <= k < n, of one-half of
 *          number of permutations of 1..n such that the differences have k
 *          runs with the same signs.
 * https://oeis.org/A008970
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  tabl, nonn, easy, nice
 * Formula:   a(n) computed from triangle T(n,k)=P(n,k)/2 * where
 *            P(n,k)=k*P(n-1,k)+2*P(n-1,k-1)+(n-k)*P(n-1,k-2), * P(n,1)=2,
 *            n>=2, 1<=k<n. * a(n) is taken as an element of this triangle: *
 *            For n>=6, let r be the unique integer such that * start(r) <= n
 *            <= end(r) * with start(r) = (r^2-3r+8)/2, end(r) = (r^2-...
 * Range:     u8 n <= 14, u16 n <= 33, u32 n <= 85, u64 n <= 221,
 *            u128 n <= 580, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    static bool initialized = false;
    static seq_t **T;           /* triangle data: T[row][k] */
    static idx_t *start_arr;    /* start(r) values */
    static idx_t *end_arr;      /* end(r)   values */
    static int r_max = 0;       /* maximum row precomputed */
    const seq_t small[6] = {0, 0, 1, 1, 2, 1};

    /* Handle small n directly */
    if (n < 6) return small[n];

    /* Precomputation */
    if (!initialized) {
        /* Determine r_max sufficient for n up to 100000 (safe for n up to ~10000) */
        idx_t max_n = 100000;               /* may be increased if needed */
        for (r_max = 4; ; ++r_max) {
            idx_t s = (r_max * (r_max - 1)) / 2 - (r_max - 4);
            if (s > max_n) break;
        }

        /* Allocate arrays */
        T = (seq_t **)malloc((r_max + 1) * sizeof(seq_t *));
        start_arr = (idx_t *)malloc((r_max + 1) * sizeof(idx_t));
        end_arr = (idx_t *)malloc((r_max + 1) * sizeof(idx_t));

        /* Compute start and end arrays */
        for (int r = 4; r <= r_max; ++r) {
            start_arr[r] = (r * (r - 1)) / 2 - (r - 4);
            end_arr[r]   = (r * (r - 1)) / 2 - (r - 4) + (r - 2);
        }

        /* Allocate triangle storage */
        for (int r = 2; r <= r_max; ++r) {
            T[r] = (seq_t *)malloc((r + 1) * sizeof(seq_t)); /* use indices 1..r-1 */
        }

        /* Temporary arrays for recurrence */
        seq_t *prev = (seq_t *)calloc(r_max + 1, sizeof(seq_t));
        seq_t *curr = (seq_t *)calloc(r_max + 1, sizeof(seq_t));

        /* n = 2 */
        prev[1] = 2;
        T[2][1] = 1;  /* 2 / 2 */

        /* Compute rows 3..r_max */
        for (int n = 3; n <= r_max; ++n) {
            curr[1] = 2;
            T[n][1] = 1;

            for (int k = 2; k <= n - 1; ++k) {
                seq_t part1 = prev[k] * (seq_t)k;
                seq_t part2 = prev[k - 1] * (seq_t)2;
                seq_t part3 = prev[k - 2] * (seq_t)(n - k);
                curr[k] = part1 + part2 + part3;
                T[n][k] = curr[k] / 2;   /* P / 2 */
            }
            /* Advance */
            for (int i = 1; i <= n; ++i) prev[i] = curr[i];
        }
        free(prev);
        free(curr);

        initialized = true;
    }

    /* Find row r such that start[r] <= n <= end[r] */
    int r = 4;
    while (r <= r_max && start_arr[r] <= n) {
        if (end_arr[r] >= n) break;
        ++r;
    }

    /* Compute k */
    idx_t d   = n - start_arr[r];
    idx_t mod = (idx_t)(r - 1);
    idx_t k0  = (d + 2) % mod;              /* 0..mod-1 */
    idx_t k   = (k0 == 0) ? mod : k0;        /* map 0 to r-1 */

    return T[r][k];
}

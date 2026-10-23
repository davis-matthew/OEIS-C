/*
 * A124943  Table read by rows: number of partitions of n with k as low
 *          median.
 * https://oeis.org/A124943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of partitions of N with k as low median, where the
 *            sequence is the table read by rows
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   index_to_Nk
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/index_to_Nk.h"

/* The build supplies seq_t and idx_t.  idx_t is an unsigned 64‑bit type. */

#define MAX_N 200                           /* enough for all required terms */

static bool initialized = false;
static idx_t curN = 0;

static seq_t *dp_atmost = NULL;             /* dp_atmost[sum][maxParts] */
static seq_t *dp_exact = NULL;              /* dp_exact[sum][parts][maxPart] */

/* Helper macros to index the flat arrays */
#define IDX2(s,p)      (((s) * (curN + 1)) + (p))
#define IDX3(s,p,m)    ((((s) * (curN + 1) + (p)) * (curN + 1)) + (m))

/* Recompute tables up to MAX_N.  Called once. */
static void compute_tables(void)
{
    /* Allocate memory */
    size_t size2 = (curN + 1) * (curN + 1);
    size_t size3 = (curN + 1) * (curN + 1) * (curN + 1);

    dp_atmost = (seq_t *)calloc(size2, sizeof(seq_t));
    dp_exact  = (seq_t *)calloc(size3, sizeof(seq_t));
    if (!dp_atmost || !dp_exact) {
        /* Allocation failed – terminate the program */
        abort();
    }

    /* dp_atmost – partitions of s using parts <= m (equivalently
       partitions into at most m parts). */
    for (idx_t m = 0; m <= curN; ++m)
        dp_atmost[IDX2(0, m)] = 1;          /* empty partition */

    for (idx_t s = 1; s <= curN; ++s) {
        for (idx_t m = 1; m <= curN; ++m) {
            seq_t val = dp_atmost[IDX2(s, m - 1)];
            if (s >= m)
                val += dp_atmost[IDX2(s - m, m)];
            dp_atmost[IDX2(s, m)] = val;
        }
    }

    /* dp_exact – partitions of s into exactly p parts, each <= m. */
    /* Base: only one way to partition 0 into 0 parts. */
    dp_exact[IDX3(0, 0, 0)] = 1;

    for (idx_t m = 1; m <= curN; ++m) {
        for (idx_t s = 0; s <= curN; ++s) {
            for (idx_t p = 0; p <= curN; ++p) {
                seq_t val = dp_exact[IDX3(s, p, m - 1)];
                if (s >= m && p >= 1)
                    val += dp_exact[IDX3(s - m, p - 1, m)];
                dp_exact[IDX3(s, p, m)] = val;
            }
        }
    }
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    if (!initialized) {
        curN = MAX_N;
        compute_tables();
        initialized = true;
    }

    /* Map the linear index to (N,k). */
    idx_t N, k;
    index_to_Nk(n, &N, &k);

    /* Prepare for summation over possible numbers of parts to the left (L). */
    seq_t total = 0;

    /* Loop over L = floor(r/2). Maximum possible L is floor(N/2). */
    for (idx_t L = 0; L <= N / 2; ++L) {

        /* ----- Odd r = 2*L + 1 (R = L) ----- */
        if (true) {
            idx_t R = L;
            idx_t remaining = N - k;                  /* sum of left and right parts */
            if (remaining >= R) {                     /* R parts need at least 1 each */
                idx_t min_left = L * k;               /* each left part ≥ k */
                idx_t max_left = remaining - R;       /* ensure enough for right parts */

                if (min_left <= max_left) {
                    for (idx_t S_left = min_left; S_left <= max_left; ++S_left) {
                        idx_t T_left = S_left - L * k;       /* after subtracting k from each left part */
                        idx_t S_right = remaining - S_left; /* sum of right parts */

                        seq_t left_cnt = dp_atmost[IDX2(T_left, L)];          /* ≤ L parts */
                        seq_t right_cnt = dp_exact
                            [IDX3(S_right, R, (idx_t)k)];                   /* exactly R parts, each ≤ k */

                        total += left_cnt * right_cnt;
                    }
                }
            }
        }

        /* ----- Even r = 2*L (R = L-1) ----- */
        if (L >= 1) {
            idx_t R = L - 1;
            idx_t remaining = N - k;
            if (remaining >= R) {
                idx_t min_left = L * k;
                idx_t max_left = remaining - R;

                if (min_left <= max_left) {
                    for (idx_t S_left = min_left; S_left <= max_left; ++S_left) {
                        idx_t T_left = S_left - L * k;
                        idx_t S_right = remaining - S_left;

                        seq_t left_cnt = dp_atmost[IDX2(T_left, L)];
                        seq_t right_cnt = dp_exact
                            [IDX3(S_right, R, (idx_t)k)];

                        total += left_cnt * right_cnt;
                    }
                }
            }
        }
    }

    return total;
}

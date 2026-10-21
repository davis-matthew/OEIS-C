/*
 * A215605  Number of unordered interval sequences that sum up to 12n in
 *          Schoenberg 12-tone rows.
 * https://oeis.org/A215605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   dfs_cycle, is_realizable
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A215605: Number of unordered interval sequences that sum up to 12 n
    in Schoenberg 12‑tone rows.
    The code enumerates all multisets of 12 interval values (1…11) whose sum is a multiple
    of 12, checks which ones are realizable by a 12‑tone row, and counts how many
    realize each sum 12 n (for 1 ≤ n ≤ 11).  For n outside this range the function
    returns 0. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/dfs_cycle.h"
#include "../utils/is_realizable.h"

/* size of the table of precomputed values */
#define MAX_N 11

/* table of results – a[0] unused */
static seq_t a_values[MAX_N + 1];
static bool initialized = false;

/* ----------------------------------------------------------------- */

/* ----------------------------------------------------------------- */
/*  Enumerates all integer vectors cnt[1…11] with sum 12.  For each
    vector it computes the sum S = Σ i·cnt[i] and, if S is a multiple
    of 12, checks whether it is realizable.  If yes, the counter
    a_values[n] is incremented where n = S/12 (1 ≤ n ≤ 11).  */
static void enumerate_counts(int idx, int remaining, int sum, int *cnt)
{
    if (idx == 12)                         /* all 11 positions processed */
    {
        if (remaining != 0) return;        /* should never happen */
        if (cnt[0] != 0) return;           /* safety check */

        if (sum % 12 != 0) return;
        int n = sum / 12;
        if (n < 1 || n > MAX_N) return;

        if (is_realizable(cnt))
            a_values[n]++;                 /* count one realizable multiset */
        return;
    }

    /* choose how many times number idx appears, from 0 up to remaining */
    for (int k = 0; k <= remaining; ++k)
    {
        cnt[idx] = k;
        enumerate_counts(idx + 1, remaining - k, sum + k * idx, cnt);
    }
}

/* ----------------------------------------------------------------- */
/*  Initializes the table of values the first time a() is called.  */
static void init_table(void)
{
    /* initialize the result array with zeros */
    for (int i = 0; i <= MAX_N; ++i) a_values[i] = 0;

    int cnt[12] = {0};
    enumerate_counts(1, 12, 0, cnt);
}

/* ----------------------------------------------------------------- */
/*  Public function required by the build system.                     */
seq_t a(idx_t n)
{
    if (!initialized)
    {
        init_table();
        initialized = true;
    }

    if (n >= 1 && n <= MAX_N)
        return a_values[(int)n];
    else
        return 0;
}

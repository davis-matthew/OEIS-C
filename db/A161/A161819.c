/*
 * A161819  a(n) = the smallest integer > n such that both n and a(n), when
 *          represented in binary, contain the same types of runs of 1's, the
 *          runs being in any order.
 * https://oeis.org/A161819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest integer > n such that the multiset of 1‑run
 *            lengths in binary representation of a(n) equals that of n
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   same_sorted_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/same_sorted_runs.h"

/* maximum number of 1‑runs we will handle (enough for the required range). */
#define MAX_RUNS 2048

/* Extract the lengths of consecutive 1‑runs from x into runs[].  The array is
   filled and *cnt is set to the number of runs.  The function modifies a copy
   of x and leaves the original unchanged. */
static void
extract_runs(seq_t x, int runs[MAX_RUNS], int *cnt)
{
    int c = 0;
    while (x) {
        /* Skip zeros. */
        while (((x & 1) == 0) && x) {
            x >>= 1;
        }
        if (!x) break;          /* no more runs */
        /* Count a run of 1's. */
        int run = 0;
        while ((x & 1) == 1) {
            run++;
            x >>= 1;
        }
        if (c < MAX_RUNS) {
            runs[c++] = run;
        }
    }
    *cnt = c;
}

/* Sort the first n elements of arr in non‑decreasing order using insertion sort. */
static void
sort_runs(int arr[], int n)
{
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

/* Main sequence function as required. */
seq_t a(idx_t n)
{
    /* Extract and sort the run lengths of n. */
    int runsN[MAX_RUNS], cntN;
    extract_runs((seq_t)n, runsN, &cntN);
    sort_runs(runsN, cntN);

    seq_t m = (seq_t)n + 1;          /* start searching from n+1 */
    while (1) {
        int runsM[MAX_RUNS], cntM;
        extract_runs(m, runsM, &cntM);
        sort_runs(runsM, cntM);
        if (same_sorted_runs(runsN, cntN, runsM, cntM)) {
            return m;
        }
        m++;                         /* increment candidate */
    }
}

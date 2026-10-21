/*
 * A061386  Sum of digits = 3 times number of digits.
 * https://oeis.org/A061386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose digit sum equals 3 times the number of digits. */

#include <stdbool.h>

/* Maximum number of digits we consider in the pre‑computation.  */
#define MAX_K   20
/* Maximum possible digit sum for MAX_K digits (all digits 9). */
#define MAX_SUM (9 * MAX_K)

static seq_t ways[MAX_K + 1][MAX_SUM + 1][2] = {0};
static bool ways_init = false;

/* Pre‑compute the number of digit strings of a given length (pos)
 * with a given remaining digit sum (sum).  first = 1 indicates that
 * the most significant digit is being chosen, hence must be ≥1. */
static void compute_ways(void)
{
    if (ways_init)
        return;

    for (int pos = 0; pos <= MAX_K; ++pos) {
        for (int sum = 0; sum <= MAX_SUM; ++sum) {
            for (int first = 0; first <= 1; ++first) {
                if (pos == 0) {
                    ways[pos][sum][first] = (sum == 0) ? 1 : 0;
                } else {
                    int low = first ? 1 : 0;
                    seq_t tot = 0;
                    for (int d = low; d <= 9; ++d) {
                        if (sum - d < 0)
                            break;
                        tot += ways[pos - 1][sum - d][0];
                    }
                    ways[pos][sum][first] = tot;
                }
            }
        }
    }
    ways_init = true;
}

/* Recursive search that returns the target‑th number (1‑based) of
 * length k and digit sum 3*k.  The global variables target_rem
 * and result_val are used to propagate the found value. */
static seq_t target_rem = 0;
static seq_t result_val = 0;

static void dfs_find(int pos, int sum, bool first, seq_t prefix)
{
    if (pos == 0) {
        if (sum == 0) {
            target_rem--;
            if (target_rem == 0)
                result_val = prefix;
        }
        return;
    }

    int low = first ? 1 : 0;
    for (int d = low; d <= 9; ++d) {
        if (sum - d < 0)
            break;
        seq_t rem_ways = ways[pos - 1][sum - d][0];
        if (target_rem > rem_ways) {
            target_rem -= rem_ways;
        } else {
            dfs_find(pos - 1, sum - d, false, prefix * 10 + d);
            return;
        }
    }
}

/* Main function to compute the n-th term of A061386. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for index 0, but return 0 */

    compute_ways();

    seq_t cumulative = 0;
    int k = 0;
    seq_t needed = n;

    while (true) {
        ++k;
        seq_t count = ways[k][3 * k][1];
        if (cumulative + count >= needed)
            break;
        cumulative += count;
    }

    target_rem = n - cumulative;   /* 1‑based index within length k */
    result_val = 0;
    dfs_find(k, 3 * k, true, 0);
    return result_val;
}

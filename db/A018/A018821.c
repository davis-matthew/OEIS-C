/*
 * A018821  n is the sum of k nonzero squares for all 2 <= k <= n-14.
 * https://oeis.org/A018821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest integer m such that m can be written as the
 *            sum of k nonzero squares for all 2 ≤ k ≤ m-14
 * Range:     u8 n <= 62, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_N 100000   /* upper bound for integers we will test */

static unsigned char cank[6][MAX_N + 1];
static bool built = false;

/* Build DP tables for representability with exactly k squares (1 ≤ k ≤ 5). */
static void build_tables(void)
{
    /* cank[0][0] is true, all others are already zero due to static init. */
    cank[0][0] = 1;

    for (idx_t s = 1; s * s <= MAX_N; ++s) {
        idx_t sq = s * s;
        for (idx_t k = 1; k <= 5; ++k) {
            for (idx_t n = sq; n <= MAX_N; ++n) {
                if (cank[k - 1][n - sq])
                    cank[k][n] = 1;
            }
        }
    }
    built = true;
}

/* Return the n-th term of sequence A018821. */
seq_t a(idx_t n)
{
    if (!built)
        build_tables();

    idx_t found = 0;
    idx_t candidate = 17;  /* first possible integer, per sequence information */

    while (1) {
        if (candidate > MAX_N) {
            /* In practice this should not happen for typical problem limits. */
            return (seq_t)candidate;
        }

        idx_t limit = (candidate <= 19) ? candidate - 14 : 5;
        bool ok = true;
        for (idx_t k = 2; k <= limit && ok; ++k) {
            if (!cank[k][candidate])
                ok = false;
        }

        if (ok) {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}

/*
 * A175453  a(1)=1. a(n) = sum a(k), where k, taken over the runs (of both 0's
 *          and 1's) in binary n, equals the length of each run.
 * https://oeis.org/A175453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(1)=1; for n>1, a(n) is the sum of a(k) for each run‑length k
 *            in the binary expansion of n
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   sum_run
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/sum_run.h"

static seq_t a_table[65];
static bool a_initialized = false;

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n<1, but safe */

    /* initialize table once */
    if (!a_initialized) {
        a_table[1] = (seq_t)1;
        for (idx_t len = 2; len <= 64; ++len) {
            a_table[len] = sum_run(len, a_table);
        }
        a_initialized = true;
    }

    return sum_run(n, a_table);
}

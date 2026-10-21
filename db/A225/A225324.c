/*
 * A225324  Partition numbers of the form 4k.
 * https://oeis.org/A225324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 32, u64 n <= 104, u128 n <= 364,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) are the partition numbers p(m) that are multiples of 4,
   listed in increasing order.  The offset is 1: a(1) = 56, a(2) = 176, … */

#include <stddef.h>
#include <stdlib.h>

/* --- Configuration ----------------------------------------------------- */

#define MAX_PARTITIONS 200000       /* enough to reach at least 10 000 terms */
#define MAX_DIV4        20000       /* sufficient for the tests */

/* --- Storage ------------------------------------------------------------ */

/* Partition numbers p(0 … max_computed), 0‑based.  p(0) = 1. */
static seq_t partition_cache[MAX_PARTITIONS];
static idx_t max_computed = 0;       /* last index that has been computed */

/* List of partition numbers that are divisible by 4, in the order of appearance. */
static seq_t div4_vals[MAX_DIV4];
static size_t div4_len = 0;          /* number of entries currently stored */

/* ----------------------------------------------------------------------- */

/* Compute partition numbers up to >= target terms divisible by 4. */
static void ensure_div4_vals(idx_t target)
{
    /* base case: p(0) = 1 */
    if (max_computed == 0) {
        partition_cache[0] = (seq_t)1;
    }

    while (div4_len < target) {
        if (max_computed + 1 >= MAX_PARTITIONS)
            abort();                /* cannot handle larger / extrapolate if needed */

        idx_t n = max_computed + 1;   /* next partition index to compute */

        seq_t sum = (seq_t)0;
        for (idx_t k = 1;; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > n)
                break;                 /* no more contributions for this n */

            seq_t term = partition_cache[n - g1];
            sum = (k & 1) ? sum + term : sum - term;

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > n)
                continue;              /* skip if beyond n */

            term = partition_cache[n - g2];
            sum = (k & 1) ? sum + term : sum - term;
        }

        partition_cache[n] = sum;

        if ((sum % 4) == 0) {
            if (div4_len >= MAX_DIV4)
                abort();                /* buffer overflow – should not occur */
            div4_vals[div4_len++] = sum;
        }

        max_computed = n;
    }
}

/* ----------------------------------------------------------------------- */

/* External function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for offset‑1 sequence */

    ensure_div4_vals(n);
    return div4_vals[n - 1];
}

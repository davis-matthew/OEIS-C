/*
 * A161000  Triangle read by rows: For 1 <= m <= n, t(n,m) = the smallest
 *          positive integer that when read in binary contains exactly (n+1-m)
 *          runs of 0's and 1's, all runs being of distinct lengths m through
 *          n in any order within binary t(n,m).
 * https://oeis.org/A161000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 28, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A161000
   t(n,m) is the smallest positive integer whose binary representation
   contains the distinct run lengths m..n, the runs alternating 0 and 1,
   starting with 1.  The sequence a(n) is obtained by reading this
   triangle row by row. */

#include <stdint.h>
#include <stdbool.h>

static inline void append_run(seq_t *value, unsigned len, bool one)
{
    if (one) {
        for (unsigned i = 0; i < len; ++i)
            *value = (*value << 1) | 1;
    } else {
        for (unsigned i = 0; i < len; ++i)
            *value <<= 1;
    }
}

seq_t a(idx_t n)
{
    /* determine the row r and column m (1‑based) */
    idx_t r = 1;
    idx_t cumulative = 1;
    while (cumulative < n) {
        ++r;
        cumulative += r;          /* cumulative == r(r+1)/2 */
    }
    idx_t prev = cumulative - r; /* start index of this row minus one */
    idx_t m = n - prev;          /* position within the row */

    /* compute t(r,m) */
    seq_t result = 0;
    unsigned cur_small = (unsigned)m;   /* next smallest unused length for 1‑runs */
    unsigned cur_large = (unsigned)r;   /* next largest unused length for 0‑runs */
    idx_t runs = r + 1 - m;             /* number of runs in this integer */

    for (idx_t i = 1; i <= runs; ++i) {
        unsigned len = (i & 1) ? cur_small++ : cur_large--;
        append_run(&result, len, (i & 1) != 0);
    }

    return result;
}

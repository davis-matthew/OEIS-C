/*
 * A161820  a(n) is the smallest positive integer such that both n and a(n),
 *          when represented in binary, contain the same types of runs of 1's,
 *          the runs being in any order.
 * https://oeis.org/A161820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 340, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = smallest positive integer m such that n and m have the
 * same multiset of run‑lengths of consecutive 1s in their binary
 * representations (the order of runs is irrelevant).
 * The implementation obtains the runs of n, sorts them increasingly,
 * then builds the numerically smallest binary string that contains
 * exactly those runs, placing one separating 0 between consecutive runs.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Step 1: collect run lengths of 1’s in n (scan from least significant) */
    unsigned runs[65];   /* at most 64 runs for a 64‑bit integer */
    size_t cnt = 0;
    idx_t temp = n;              /* use idx_t for the shift loop */
    while (temp > 0) {
        /* skip zeros between runs */
        while ((temp & 1) == 0) {
            temp >>= 1;
        }
        /* count consecutive 1s */
        unsigned len = 0;
        while ((temp & 1) == 1) {
            ++len;
            temp >>= 1;
        }
        runs[cnt++] = len;
    }

    /* Step 2: sort run lengths ascending (insertion sort, trivial size) */
    for (size_t i = 1; i < cnt; ++i) {
        unsigned key = runs[i];
        size_t j = i;
        while (j > 0 && runs[j - 1] > key) {
            runs[j] = runs[j - 1];
            --j;
        }
        runs[j] = key;
    }

    /* Step 3: construct the minimal integer from the sorted runs */
    seq_t result = 0;
    const size_t bitcount = 8 * sizeof(seq_t);   /* number of bits in seq_t */

    for (size_t i = 0; i < cnt; ++i) {
        unsigned run = runs[i];
        seq_t mask;
        if (run == bitcount)              /* avoid shifting by the type width */
            mask = (seq_t)(-1);
        else
            mask = ((seq_t)1 << run) - 1; /* a block of run 1's */

        result = (result << run) | mask;  /* append the block of 1's */
        if (i + 1 < cnt)
            result <<= 1;                 /* separate runs by a single 0 */
    }

    return result;
}

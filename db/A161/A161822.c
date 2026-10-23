/*
 * A161822  a(n) = the smallest positive integer such that both n and a(n),
 *          when represented in binary, contain the same types of runs of 0's,
 *          the runs being in any order.
 * https://oeis.org/A161822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the smallest positive integer whose binary representation
   contains the same multiset of zero‑run lengths as that of n. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Comparator: sort run lengths in decreasing order. */
static int cmp_desc(const void *a, const void *b)
{
    idx_t va = *(const idx_t *)a;
    idx_t vb = *(const idx_t *)b;
    /* return positive if va < vb so that qsort puts larger values first */
    return (va < vb) - (va > vb);
}

seq_t a(idx_t n)
{
    /* Gather all zero‑run lengths of n. */
    idx_t run_counts[128];
    size_t run_k = 0;
    idx_t zero_run = 0;

    /* Determine number of bits in n (msb position + 1). */
    idx_t bitlen = 0;
    {
        idx_t tmp = n;
        while (tmp) {
            bitlen++;
            tmp >>= 1;
        }
        if (bitlen == 0) bitlen = 1;          /* for n == 0, not needed */
    }

    /* Scan bits from msb to lsb. */
    idx_t i = bitlen;
    while (i--) {
        bool bit = (n >> i) & 1;
        if (bit) {
            if (zero_run > 0) {
                run_counts[run_k++] = zero_run;
                zero_run = 0;
            }
        } else {
            zero_run++;
        }
    }
    if (zero_run > 0) {
        run_counts[run_k++] = zero_run;
    }

    /* If there are no zero runs, the answer is 1 (all ones). */
    if (run_k == 0) {
        return (seq_t)1;
    }

    /* Sort runs in decreasing order to obtain the minimal number. */
    qsort(run_counts, run_k, sizeof(idx_t), cmp_desc);

    /* Build the minimal binary number from the sorted runs. */
    seq_t res = (seq_t)1;
    for (size_t j = 0; j < run_k; j++) {
        /* Shift left by the run length (multiply by 2 that many times). */
        for (idx_t s = 0; s < run_counts[j]; ++s) {
            res *= (seq_t)2;
        }
        /* Append the next '1' unless this is the last run. */
        if (j + 1 < run_k) {
            res = (res * (seq_t)2) | (seq_t)1;
        }
    }
    return res;
}

/*
 * A175262  Those positive integers that when written in binary contain an odd
 *          number of digits, the middle digit being a 1.
 * https://oeis.org/A175262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the n-th positive integer whose binary representation
 *            has odd length and the middle bit is 1. The function groups
 *            numbers by bit‑length and enumerates them in increasing order
 * Range:     u8 n <= 43, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;   /* the first (and only) 1‑bit number */

    idx_t   k          = 0;          /* current bit‑length group (k>=1) */
    seq_t   pow4        = 1;          /* 4^k */
    seq_t   cumulative  = 0;          /* sum of group sizes up to current k */
    seq_t   group_count = 0;          /* size of the current group */

    /* Find the k such that the n-th element lies in this group. */
    for (;;)
    {
        if (k == 0)
            group_count = 1;                      /* only the number 1 */
        else
            group_count = (seq_t)2 * (pow4 >> 2);  /* 2·4^(k‑1) */

        cumulative += group_count;               /* total up to this group */

        if ((seq_t)n <= cumulative) break;       /* n is inside this group */

        k++;
        pow4 <<= 2;                              /* multiply 4^k by 4 */
    }

    seq_t prev_cumulative = cumulative - group_count;
    seq_t offset          = (seq_t)n - prev_cumulative;   /* 1‑based in group */
    seq_t idx_local       = offset - 1;                  /* zero‑based */

    /* Split idx_local into the bits before and after the middle bit. */
    seq_t low_mask  = ((seq_t)1 << k) - 1;   /* lower k bits */
    seq_t low_part  = idx_local & low_mask;
    seq_t high_part = idx_local >> k;        /* the remaining k‑1 bits */

    /* Construct the number from its bit components. */
    seq_t value = low_part
                | ((seq_t)1 << k)                  /* middle bit */
                | (high_part << (k + 1))           /* bits above the middle */
                | ((seq_t)1 << (2 * k));           /* most significant bit */

    return value;
}

/*
 * A227182  Simple self-inverse permutation of natural numbers: List each
 *          block of n^2 - n + 1 numbers from ((n-1)^3 + 2*(n-1))/3 + 1 to
 *          (n^3 + 2*n)/3 in reverse order.
 * https://oeis.org/A227182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n = 0; otherwise let m be the unique integer with
 *            m(m^2+2)/3 >= n > (m-1)((m-1)^2+2)/3. Then a(n) = (m^3+2m)/3 -
 *            (n - ((m-1)^3+2(m-1))/3 - 1)
 * Range:     u8 n <= 249, u16 n <= 9951, u32 n <= 9951, u64 n <= 9951,
 *            u128 n <= 9951, big any n
 * Helpers:   block_end, block_start
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the definition:
   a(n) = reverse block of length n^2 - n + 1 from
   ((n-1)^3+2(n-1))/3+1 to (n^3+2n)/3, with a(0)=0. */

#include <stdint.h>
#include "../utils/block_end.h"
#include "../utils/block_start.h"

static idx_t find_block(idx_t n)
{
    /* Find smallest m such that block_end(m) >= n */
    idx_t low  = 1;
    idx_t high = 1;

    /* Increase high until it satisfies the inequality */
    while ((seq_t)high * ((seq_t)high * (seq_t)high + 2) / 3 < (seq_t)n)
    {
        if (high > UINT64_MAX / 2) /* avoid overflow */
            break;
        high *= 2;
    }

    /* Binary search between low (inclusive) and high (inclusive) */
    while (low < high)
    {
        idx_t mid = low + ((high - low) >> 1);
        if ((seq_t)mid * ((seq_t)mid * (seq_t)mid + 2) / 3 >= (seq_t)n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t m = find_block(n);               /* block number (>=1) */
    seq_t start = block_start(m);          /* index of first element in this block */
    seq_t offset = (seq_t)(n - (idx_t)start); /* zero‑based offset in block */
    seq_t e = block_end(m);                /* last (and first reversed) value in block */

    return e - offset;
}

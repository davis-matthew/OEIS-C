/*
 * A218254  Irregular table, where row n (n >= 0) starts with n, the next term
 *          is n-A000120(n), and the successive terms are obtained by
 *          repeatedly subtracting the number of 1's in the previous term's
 *          binary expansion, until zero is reached, after which the next row
 *          starts with one larger n.
 * https://oeis.org/A218254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 8996, u16 n <= 8996, u32 n <= 8996, u64 n <= 8996,
 *            u128 n <= 8996, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A218254: Irregular table concatenation
   (row n starts with n, then subtracts the popcount of the previous term
   repeatedly until 0, then next row starts).
   The function returns the nth term of the concatenated rows. */
#include <stdlib.h>

/* popcount for an arbitrary-size unsigned integer type seq_t.
   Assumes seq_t supports & and >> operators. */
static unsigned int popcount_seq(seq_t x)
{
    unsigned int c = 0;
    while (x != 0) {
        c += (unsigned int)(x & 1);
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* Static table of cumulative term counts per row:
       cum[i] = total number of terms in rows 0..i inclusive. */
    static idx_t *cum = NULL;          /* array of cumulative totals */
    static size_t cum_cap = 0;         /* allocated capacity */
    static size_t cum_size = 0;        /* number of rows stored */
    static idx_t cum_total = 0;        /* current cumulative total */

    /* Grow cum until it covers the requested index n. */
    while (cum_total <= n) {
        idx_t r = (idx_t)cum_size;    /* next row index */

        /* Compute the length of row r. */
        seq_t val = (seq_t)r;
        idx_t len = 1;                /* first term counted */
        seq_t t = val;
        while (t > 0) {               /* include zero term in count */
            t -= (seq_t)popcount_seq(t);
            len++;
        }

        /* Allocate or grow the cum array. */
        if (cum_size + 1 > cum_cap) {
            size_t new_cap = cum_cap == 0 ? 4 : cum_cap * 2;
            idx_t *tmp = (idx_t *)realloc(cum, new_cap * sizeof(idx_t));
            if (!tmp) abort();        /* allocation failure */
            cum = tmp;
            cum_cap = new_cap;
        }

        cum_total += len;
        cum[cum_size] = cum_total;
        cum_size++;
    }

    /* Binary search to find the row containing index n. */
    size_t lo = 0, hi = cum_size - 1;
    idx_t row = hi;
    while (lo <= hi) {
        size_t mid = (lo + hi) / 2;
        if (cum[mid] > n) {
            row = (idx_t)mid;
            if (mid == 0) break;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }

    /* Position within the located row (0‑based). */
    idx_t pos;
    if (row == 0)
        pos = n;
    else
        pos = n - cum[row - 1];

    /* Generate the terms of row `row` up to `pos`. */
    seq_t term = (seq_t)row;
    if (pos == 0) return term;

    for (idx_t i = 1; i <= pos; ++i) {
        term -= (seq_t)popcount_seq(term);
        if (i == pos) return term;
    }

    /* Control should never reach here. */
    return term;
}

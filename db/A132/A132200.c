/*
 * A132200  Numbers in (4,4)-Pascal triangle .
 * https://oeis.org/A132200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = element in the (4,4)-Pascal triangle * read in row‑major
 *            order. * * The triangle is defined by T(0,0)=1 and * T(n,k) = 4
 *            * binomial(n,k) for n>0, 0<=k<=n. * The sequence a(n) lists the
 *            values T(r,k) where * r = floor( (sqrt(8n+1)-1)/2 ) is the row
 *            index and * k = n - r*(r+1)/2 is the colum...
 * Range:     u8 n <= 39, u16 n <= 159, u32 n <= 575, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* --------- helper: multiplicative binomial for seq_t --------- */
static seq_t
_binom(idx_t n, idx_t k)
{
    /* Reduce the number of multiplicative steps */
    if (k > n - k) k = n - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* (n - k + i) fits in idx_t; cast to seq_t for multiplication */
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* --------- find row index r for a given linear index n --------- */
static idx_t
_find_row(idx_t n)
{
    /* Find the smallest r such that (r+1)*(r+2)/2 > n.
     * Equivalent to r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t low  = 0;
    idx_t high = 1;

    /* Exponentially increase high until it bounds n */
    while ((high * (high + 1) / 2) <= n) {
        high <<= 1;
    }

    /* Binary search between low and high */
    while (low < high) {
        idx_t mid = (low + high + 1) / 2;
        if ((mid * (mid + 1) / 2) <= n) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }

    return low;
}

/* --------- main sequence function --------- */
seq_t
a(idx_t n)
{
    idx_t r = _find_row(n);            /* row index */
    idx_t k = n - r * (r + 1) / 2;     /* column index within row */

    if (r == 0) {                      /* T(0,0) = 1 */
        return (seq_t)1;
    }

    seq_t bin = _binom(r, k);          /* binomial coefficient */
    return 4 * bin;                    /* T(r,k) = 4 * C(r,k) */
}

/*
 * A090774  A permutation of the natural numbers related to sequence A079946.
 * https://oeis.org/A090774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   read as a square array by antidiagonals (column j of each
 *            diagonal first); column j is a triangle whose row m (m>=1) has
 *            entries T(m,k) = (c_j+1)*2^(m-1) - 1 - (2j-1)*(2^(m-1) -
 *            2^(m-1-k)), k=0..m-1, where c_1 = 1 and c_j = A079946(j-1) for
 *            j>=2, A079946(n) = 2^floor(log_2(4n)) + 2n
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A090774_A079946(idx_t n)
{
    idx_t t = 4 * n;
    unsigned int p = 0;
    while ((t >> (p + 1)) != 0) {
        p++;
    }
    seq_t two_pow = ((seq_t)1) << (int)p;
    return two_pow + (seq_t)(2 * n);
}

seq_t a(idx_t n)
{
    /* diagonal d: d(d-1)/2 < n <= d(d+1)/2 ; j = position in the diagonal = column */
    idx_t d = 1;
    while ((d * (d + 1)) / 2 < n) {
        d++;
    }
    idx_t j = n - (d * (d - 1)) / 2;
    idx_t t = d - j + 1;            /* 1-based index within column triangle */

    idx_t m = 1;
    while ((m * (m + 1)) / 2 < t) {
        m++;
    }
    idx_t k = t - (m * (m - 1)) / 2 - 1;

    seq_t c;
    if (j == 1) {
        c = (seq_t)1;
    } else {
        c = A090774_A079946(j - 1);
    }

    seq_t pw = ((seq_t)1) << (int)(m - 1);
    seq_t pwk = ((seq_t)1) << (int)(m - 1 - k);
    seq_t top = (c + 1) * pw - 1;
    seq_t sub = ((seq_t)2 * (seq_t)j - 1) * (pw - pwk);
    return top - sub;
}

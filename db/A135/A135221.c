/*
 * A135221  Triangle A007318 + A000012(signed) - I, I = Identity matrix, read
 *          by rows.
 * https://oeis.org/A135221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the triangle T(r,k) = (-1)^(r-k) +
 *            binomial(r,k) for k < r, and T(r,r) = 1. The triangle is read by
 *            rows; the row index r satisfies r(r+1)/2 ≤ n < (r+1)(r+2)/2, and
 *            c = n – r(r+1)/2 is the column index (0‑based). Then a(n) =
 *            T(r,c)
 * Range:     u8 n <= 69, u16 n <= 197, u32 n <= 646, u64 n <= 2376,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiplicative binomial coefficient using 128‑bit intermediates
   for 8‑byte or smaller seq_t, otherwise using seq_t arithmetic directly */
static seq_t binom_value(idx_t n, idx_t k)
{
    idx_t k2 = k;
    if (k2 > n - k2)
        k2 = n - k2;
    seq_t res = 1;
    for (idx_t i = 1; i <= k2; ++i) {
        if (sizeof(seq_t) <= 8) {
            /* use 128‑bit to avoid overflow of the intermediate product */
            unsigned __int128 tmp = (unsigned __int128)res * (n - i + 1);
            res = (seq_t)(tmp / i);
        } else {
            /* seq_t is neither 8‑byte nor smaller (big integer or 128‑bit) */
            res = (seq_t)(res * (n - i + 1) / i);
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* find row r and number of elements before it (sum) */
    idx_t r = 0;
    idx_t sum = 0;
    while (1) {
        idx_t next = r + 1;
        if (sum + next > n)
            break;
        sum += next;
        ++r;
    }
    /* column index within the row */
    idx_t c = n - sum;
    if (c == r)
        return (seq_t)1; /* diagonal entry */

    seq_t bin = binom_value(r, c);
    /* parity of (r-c) determines the sign of the ±1 term */
    if (((r - c) & 1U) == 0)
        return bin + (seq_t)1;
    else
        return bin - (seq_t)1;
}

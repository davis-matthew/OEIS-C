/*
 * A117438  Triangle T(n, k) = binomial(2*n-k, k)*(-4)^(n-k), read by rows.
 * https://oeis.org/A117438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = binomial(2r - k, k) * (-4)^(r-k) where r =
 *            floor((sqrt(8n+1)-1)/2) is the row, k = n - r(r+1)/2 is the
 *            column
 * Range:     i8 n <= 9, i16 n <= 28, i32 n <= 106, i64 n <= 356,
 *            i128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t if needed */

/* Local binomial coefficient using integer arithmetic.
   Assumes n >= k. */
static seq_t
local_binomial(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply then divide; the division is exact. */
        res = res * (n - k + i) / i;
    }
    return res;
}

/* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2.
   This routine is O(sqrt(n)) but adequate for n up to at least 10^9
   (and far faster for the expected n <= 10000). */
static idx_t
find_row(idx_t n)
{
    idx_t r = 0;
    idx_t terms_before = 0;          /* r(r+1)/2 */
    while (1) {
        idx_t next_terms = terms_before + (r + 1);  /* terms up to row r */
        if (next_terms > n) break;
        terms_before = next_terms;
        ++r;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Determine row and column. */
    idx_t r = find_row(n);
    idx_t terms_before = r * (r + 1) / 2;
    idx_t k = n - terms_before;      /* 0 <= k <= r */

    /* Compute binomial(2r - k, k). */
    seq_t binom = local_binomial(2 * r - k, k);

    /* Compute (-4)^(r-k) by repeated multiplication (r-k <= r). */
    seq_t pow_n = 1;
    for (idx_t i = 0; i < r - k; ++i)
        pow_n *= (seq_t)-4;

    /* Result is product. */
    return binom * pow_n;
}

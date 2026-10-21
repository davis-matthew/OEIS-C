/*
 * A096754  Triangle read by rows giving coefficients of the trigonometric
 *          expansion of Cos(n*x).
 * https://oeis.org/A096754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = Let r be the unique integer such that S(r-1) < n ≤ S(r),
 *            where S(r) = (r+1) if r is even, S(r) = r if r is odd. Let k = n
 *            - S(r-1) - 1 (0‑based position inside the row). Then a(n) = 0 if
 *            k is odd, (-1)^(k/2) * binom(r, k) otherwise. (binom(r, k) is
 *            the standard binomial coefficient.)
 * Range:     i8 n <= 53, i16 n <= 89, i32 n <= 89, i64 n <= 89, i128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */

/* Helper: compute n choose k as seq_t */
static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* multiply then divide; division is exact */
        res = res * (n - k + i) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but guard */

    /* Find row r such that cumulative count S(r-1) < n <= S(r) */
    idx_t r = 1;
    idx_t cumulative = 0;
    while (1)
    {
        idx_t row_len = (r % 2 == 0) ? (r + 1) : r; /* exclude trailing zero if odd */
        if (cumulative + row_len >= n)
            break;
        cumulative += row_len;
        ++r;
    }

    /* Position within the row (0‑based) */
    idx_t pos = n - cumulative - 1;   /* 0 ≤ pos < row_len */
    idx_t k = pos;  /* k directly corresponds to pos because we removed trailing zero */

    /* Compute coefficient */
    if (k & 1)   /* odd k => coefficient zero */
        return 0;

    seq_t bin = binom_idx(r, k);
    /* sign: (-1)^(k/2) */
    if ((k / 2) & 1)
        return -bin;
    else
        return bin;
}

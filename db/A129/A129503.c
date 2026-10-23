/*
 * A129503  Pascal's Fredholm-Rueppel triangle.
 * https://oeis.org/A129503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = 0 if the column index in the antidiagonal ordering is
 *            not a power of two, * otherwise a(n) = binomial(r + e - 2^e, e)
 *            * where * r = row index (0‑based) of the n‑th element in the
 *            triangle, * e = integer such that column index j = 2^e, * j =
 *            offset within the row + 1 (1‑based column numbe...
 * Range:     u8 n <= 160, u16 n <= 1096, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: small binomial coefficient for non‑negative n and k (k <= n),
 * computed multiplicatively to stay exactly in integer arithmetic. */
static seq_t small_binom(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* res = res * (n - k + i) / i; */
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the row r (0‑based) such that
     *   sum_{i=0}^r (i+1) >= n > sum_{i=0}^{r-1} (i+1).
     */
    idx_t r = 0;
    idx_t total = 1;                  /* sum up to row r */
    while (total < n) {
        ++r;
        total += (r + 1);
    }
    idx_t prev_total = total - (r + 1);   /* total up to row r-1 */
    idx_t offset     = n - prev_total - 1;/* 0‑based offset in row r */
    idx_t j          = offset + 1;        /* 1‑based column index */

    /* Column must be a power of two. */
    if ((j & (j - 1)) != 0)
        return 0;

    /* Compute e such that j = 2^e. */
    idx_t e   = 0;
    idx_t pow2 = 1;
    while (pow2 < j) {
        ++e;
        pow2 <<= 1;
    }

    /* Row number for the binomial formula is 1‑based. */
    idx_t rownum = r + 1;

    /* Argument to the binomial is rownum + e - 2^e. */
    idx_t arg = rownum + e - pow2;
    /* binomial(arg, e) is always defined with arg >= e. */
    return small_binom(arg, e);
}

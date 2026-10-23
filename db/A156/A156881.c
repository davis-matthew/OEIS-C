/*
 * A156881  Square array T(n, k) = Product_{j=1..n} ( Sum_{i=0..j-1} ((k+1)^2
 *          - (k+1))^i ) with T(n, 0) = n!, read by antidiagonals.
 * https://oeis.org/A156881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(n-i, i) where i = n - offset of greatest s with
 *            s*(s+1)/2 <= n T(n, k) = Prod_{j=1..n} ( Sum_{i=0}^{j-1}
 *            ((k+1)^2-(k+1))^i ) For k = 0 we define T(n,0) = n! (factorial)
 * Range:     u8 n <= 17, u16 n <= 24, u32 n <= 40, u64 n <= 60, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute factorial for small n (used only when k==0) */
static seq_t fact(idx_t m)
{
    seq_t r = 1;
    for (idx_t i = 1; i <= m; ++i)
        r *= (seq_t)i;
    return r;
}

/* compute T(n,k) for n>=0, k>=0  */
static seq_t compute_T(idx_t n, idx_t k)
{
    if (k == 0)          /* column zero -> factorial */
        return fact(n);
    if (n == 0)          /* row zero -> product of zero terms = 1 */
        return 1;

    seq_t X = (seq_t)k * ((seq_t)k + 1);   /* X = k(k+1) */
    seq_t sum = 1;                         /* sum for j=1 */
    seq_t prod = 1;
    for (idx_t j = 1; j <= n; ++j) {
        prod *= sum;                 /* multiply by current sum */
        if (j < n)                    /* prepare sum for next j */
            sum = sum * X + 1;        /* sum_{i=0}^{j} X^i = old_sum*X + 1 */
    }
    return prod;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* find greatest s with s*(s+1)/2 <= n */
    idx_t s = 0;
    idx_t offset = 0;
    while (offset + (s + 1) <= n) {
        ++s;
        offset += s;
    }

    idx_t row = n - offset;   /* n */
    idx_t col = s - row;      /* k = s - i */

    return compute_T(row, col);
}

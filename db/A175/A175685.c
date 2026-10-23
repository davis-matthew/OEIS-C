/*
 * A175685  Array a(n,m) = Sum_{j=floor((n-1)/2)-m..floor(n-1)/2}
 *          binomial(n-j-1,j) read by antidiagonals.
 * https://oeis.org/A175685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is obtained by reading the array 
 *   a(n,m) = sum_{j = floor((n-1)/2)-m .. floor((n-1)/2)} C(n-j-1, j) 
 *   by antidiagonals with decreasing row index (i.e., increasing column). 
 * The function below computes the nth term (1‑based) of that sequence.
 */

#include <stdint.h>
#include <stddef.h>

/* multiplicative binomial coefficient for idx_t arguments, using seq_t arithmetic */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    return res;
}

/* compute a(n, m) as defined in the OEIS entry */
static seq_t a_nm(idx_t n, idx_t m)
{
    idx_t t = (n - 1) / 2;          /* floor((n-1)/2) */
    idx_t start = (t >= m) ? (t - m) : 0;  /* lower bound, clamped to 0 */
    seq_t sum = 0;
    for (idx_t j = start; j <= t; ++j)
        sum += binom(n - j - 1, j);
    return sum;
}

/* public function required by the build system */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for 0, but harmless */

    idx_t count = 0;
    for (idx_t d = 1; ; ++d) {    /* diagonal index (n + m - 1 == d) */
        for (idx_t row = d; row >= 1; --row) {   /* decreasing row */
            idx_t m = d - row;                     /* corresponding column */
            seq_t val = a_nm(row, m);
            ++count;
            if (count == n)
                return val;
        }
    }
}

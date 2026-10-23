/*
 * A121462  Triangle read by rows: T(n,k) is the number of nondecreasing Dyck
 *          paths of semilength n, having pyramid weight k (1 <= k <= n).
 * https://oeis.org/A121462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) gives the (n‑th) entry of the triangle A121462.
   For row r and column c:
        - T(1,1) = 1
        - T(r,1) = 0  (r > 1)
        - T(r,r) = 2^(r-1)
        - otherwise  T(r,c) = Σ_{j=1..c-1} binom(c-1,j) * binom(r-c-1 + j, j-1)
*/

#include <stdint.h>

/* compute binomial coefficient binom(n,k) for 0 <= k <= n
   The result is exact as long as it fits into seq_t.
   For n and k large, overflow may occur but this is acceptable for
   terms that exceed the 64‑bit limit. */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;  /* division is exact */
    }
    return res;
}

/* fast exponentiation of 2^exp (exp >= 0) */
static seq_t pow2(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 2;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* out of offset, value irrelevant */

    /* find row r such that n lies in row r */
    idx_t r = 1;
    idx_t cum = 1;                       /* cumulative entries up to current row */
    while (n > cum) {
        ++r;
        cum += r;
    }

    /* column within the row */
    idx_t prev_sum = r * (r - 1) / 2;     /* entries before row r */
    idx_t c = n - prev_sum;              /* 1‑based column */

    /* special cases */
    if (r == 1) return 1;
    if (c == 1) return 0;
    if (c == r) return pow2(r - 1);

    /* general case */
    seq_t sum = 0;
    for (idx_t j = 1; j < c; ++j) {
        seq_t term1 = binom(c - 1, j);
        seq_t term2 = binom((r - c - 1) + j, j - 1);
        sum += term1 * term2;
    }
    return sum;
}

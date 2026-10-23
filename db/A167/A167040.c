/*
 * A167040  Triangle T(n, k) = (n-k)^n * binomial(n, n-k) for n < 2*k, k^n *
 *          binomial(n, k) for n >= 2*k with T(n, 0) = T(n, n) = 1, read by
 *          rows.
 * https://oeis.org/A167040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where T(n,k) = (n-k)^n * binomial(n, n-k) for n <
 *            2*k, k^n * binomial(n,k) for n >= 2*k, T(n,0)=T(n,n)=1. The
 *            sequence is read row‑by‑row from the triangle
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 70, u64 n <= 160,
 *            u128 n <= 418, big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

static seq_t int_pow(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    idx_t sum = 0;   /* number of elements before row r */
    while (1) {
        idx_t next_sum = sum + r + 1;
        if (n < next_sum)
            break;
        sum = next_sum;
        r++;
    }
    idx_t k = n - sum;  /* column in the row */
    if (k == 0 || k == r)
        return (seq_t)1;

    if (r < 2 * k) {
        seq_t base = (seq_t)(r - k);
        seq_t powv = int_pow(base, r);
        seq_t bin = binomial(r, r - k);
        return powv * bin;
    } else {
        seq_t base = (seq_t)k;
        seq_t powv = int_pow(base, r);
        seq_t bin = binomial(r, k);
        return powv * bin;
    }
}

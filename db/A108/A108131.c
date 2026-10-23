/*
 * A108131  Array read by antidiagonals: A(k,n) = C(n^k, n).
 * https://oeis.org/A108131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial (ncol^k, ncol) where (k,ncol) are obtained from
 *            n by reading the array A(k,n)=C(n^k,n) along its antidiagonals
 * Range:     u8 n <= 17, u16 n <= 23, u32 n <= 39, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute base^exp as seq_t (arbitrary precision supported by seq_t). */
static seq_t pow_seq_local(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp) {
        res *= base;
        --exp;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the antidiagonal (sum t) that contains the index n.
       The total number of terms up to antidiagonal t is t(t+1)/2. */
    idx_t t = 1;
    while (t * (t + 1) / 2 <= n) {
        ++t;
    }

    idx_t prev = (t - 1) * t / 2;          /* terms before antidiagonal t */
    idx_t offset = n - prev;               /* index within this antidiagonal */
    idx_t k = t - offset;                  /* row index (k ≥ 1) */
    idx_t ncol = offset;                   /* column index (n ≥ 0) */

    /* Compute C(ncol^k, ncol) multiplicatively.
       Since ncol ≤ 141 for n ≤ 10000, this loop is very small. */
    if (ncol == 0) {
        return (seq_t)1;                   /* C(0,0)=1 and C(0^k,0)=1 */
    }

    seq_t pw = pow_seq_local((seq_t)ncol, k);   /* ncol^k */

    seq_t res = 1;
    for (idx_t i = 1; i <= ncol; ++i) {
        res = (res * (pw - (seq_t)(i - 1))) / (seq_t)i;
    }

    return res;
}

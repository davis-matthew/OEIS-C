/*
 * A173118  Triangle T(n, k, q) = q*[n=2] + Sum_{j=0..5} q^j*binomial(n-2*j,
 *          k-j)*[n>2*j] with T(n,0) = T(n,n) = 1 for q = 2, read by rows.
 * https://oeis.org/A173118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, less
 * Formula:   a(n) = T(r,k,2), where r and k satisfy n = r(r+1)/2 + k. * The
 *            triangle entry is defined by * T(n,k,2) = 2*[n==2] + Σ_{j=0..5}
 *            2^j * binomial(n-2*j, k-j) (0<k<n), * with T(n,0)=T(n,n)=1. *
 *            The first terms are:
 *            1,1,1,1,4,1,1,5,5,1,1,6,10,6,1,1,7,20,20,7,1,1,8,27,
 * Range:     u8 n <= 7, u16 n <= 178, u32 n <= 609, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square‑root for seq_t (non‑negative). */
static inline seq_t isqrt_seq_t(seq_t x)
{
    seq_t l = 0, r = (x >> 1) + 1;
    while (l <= r) {
        seq_t m = (l + r) >> 1;
        seq_t m2 = m * m;
        if (m2 == x)
            return m;
        if (m2 < x)
            l = m + 1;
        else
            r = m - 1;
    }
    return r;
}

/* Binomial coefficient binom(n,k) for non‑negative integers.  Uses
 * the multiplicative formula and stays within seq_t for intermediate
 * steps.  Assumes k <= n. */
static inline seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k == 0 || k == n)
        return (seq_t)1;
    if (k > n - k)
        k = n - k;                 /* symmetry */

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* res = res * (n - k + i) / i; exact division. */
        res = (res * (seq_t)(n - k + i)) / (seq_t)i;
    }
    return res;
}

/* Main sequence function.  Computes the n‑th entry of the flattened
 * triangle for q = 2. */
seq_t a(idx_t n)
{
    /* Locate the row r and column k of the triangle corresponding
     * to the linear index n.  Solve r(r+1)/2 <= n < (r+1)(r+2)/2. */
    seq_t disc = 8 * (seq_t)n + 1;
    seq_t sq   = isqrt_seq_t(disc);
    idx_t r = (idx_t)((sq - 1) / 2);
    seq_t offset = (seq_t)r * ((seq_t)r + 1) / 2;
    idx_t k = n - (idx_t)offset;

    /* Boundary values: first and last entry of every row equal 1. */
    if (k == 0 || k == r)
        return (seq_t)1;

    seq_t sum = 0;

    /* Special term q*[n==2] where q = 2 and n refers to the row index r. */
    if (r == 2)
        sum += (seq_t)2;

    /* Main finite sum over j = 0..5.  Only terms with r > 2*j are used. */
    for (int j = 0; j <= 5; ++j) {
        if ((idx_t)r <= 2 * (idx_t)j)         /* [r > 2*j] */
            continue;
        if ((idx_t)k < (idx_t)j)              /* k-j must be non‑negative */
            continue;

        idx_t nn = r - 2 * (idx_t)j;         /* n - 2*j */
        idx_t kk = k - (idx_t)j;             /* k - j */

        if (kk > nn)
            continue;                        /* need to be valid binomial */

        seq_t term = binom_idx(nn, kk);         /* binomial(nn, kk) */
        term *= ((seq_t)1 << j);               /* multiply by 2^j */
        sum += term;
    }

    return sum;
}

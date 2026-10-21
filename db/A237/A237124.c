/*
 * A237124  Triangle of numbers related to Catalan numbers (A000108).
 * https://oeis.org/A237124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 77, u32 n <= 209, u64 n <= 495,
 *            u128 n <= 495, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A237124: Flattened triangle of numbers related to Catalan numbers. */
/* a(n) = T(r,k) where r = floor((sqrt(8n+1)-1)/2) and k = n - r(r+1)/2.   */
/* The triangle entries are defined by the closed‐form:              */
/* T(r,k) = C(2r+1-k , r-k) - 2*C(2r-k , r-k-1) - C(2r-1-k , r-k-2)   */
/*          + 3*C(2r-2-k , r-k-3) - 2*C(2r-3-k , r-k-4)  (for r>2).    */
/* For r ≤ 2 the entries are just binomial coefficients C(r,k).       */
#include <stdint.h>

/* Compute a binomial coefficient C(n,k) for any idx_t range.
   Uses a 128‑bit intermediate to avoid overflow on 64‑bit builds.   */
static inline seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    unsigned __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* res = res * (n - k + i) / i;  intermediate stays in 128 bits */
        res = res * ((unsigned __int128)(n - k + i)) / i;
    }
    return (seq_t)res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    idx_t start = r * (r + 1) / 2;      /* first index of this row */
    idx_t k = n - start;                 /* column within the row (0 <= k <= r) */

    /* For the first three rows the entries are simple binomial coefficients. */
    if (r <= 2) return binom(r, k);

    /* Compute the five terms in the formula.  If an index is negative we
       treat the corresponding binomial as zero. */
    seq_t t0 = binom(2 * r + 1 - k, r - k);
    seq_t t1 = binom(2 * r     - k, r - k - 1);
    seq_t t2 = binom(2 * r - 1 - k, r - k - 2);
    seq_t t3 = binom(2 * r - 2 - k, r - k - 3);
    seq_t t4 = binom(2 * r - 3 - k, r - k - 4);

    seq_t pos = t0 + (seq_t)3 * t3;
    seq_t neg = (seq_t)2 * t1 + t2 + (seq_t)2 * t4;

    return pos - neg;
}

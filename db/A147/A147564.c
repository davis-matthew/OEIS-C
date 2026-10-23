/*
 * A147564  Triangle, T(n, k) = coefficients [x^k]( p(x, n) ), where p(x, n) =
 *          (1+x)^n +2*x*[n=2] for n < 3, otherwise (1+x)^n +
 *          2*(n*x+1)*(1+x)^(n-2) -2, read by rows.
 * https://oeis.org/A147564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   for n ≥ 3, T(n,k) = C(n,k) + 2*C(n‑2,k) + 2*n*C(n‑2,k‑1) –
 *            2·[k=0]; triangle rows (flattened) give the sequence a(n)
 * Range:     u8 n <= 38, u16 n <= 141, u32 n <= 509, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Helper: offset of the first element of row r in the flattened
   sequence.                                               */
static idx_t offset_row(idx_t r)
{
    return (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
}

/* ------------------------------------------------------------------ */
/* Helper: given a flattened index n, find the row r such that
   offset_row(r) ≤ n < offset_row(r+1).  Binary search on r.   */
static idx_t row_from_index(idx_t n)
{
    idx_t low = 0;
    idx_t high = n + 1;   /* r cannot exceed n. */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (offset_row(mid) <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

/* ------------------------------------------------------------------ */
/* Local binomial coefficient.  Works for all n, k that fit in seq_t
   (or produce values that fit after truncation).  Uses 128‑bit
   intermediate to avoid overflow of intermediate products. */
static seq_t binom_local(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;    /* use symmetry */
    seq_t res = (seq_t)1;        /* start with 1 */

    for (idx_t i = 1; i <= k; ++i) {
        unsigned __int128 tmp = (unsigned __int128)res;
        tmp *= (unsigned __int128)(n - k + i);
        tmp /= i;
        res = (seq_t)tmp;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.                                   */
seq_t a(idx_t n)
{
    /* Identify row r and column k. */
    idx_t r = row_from_index(n);
    idx_t k = n - offset_row(r);

    /* Handle the first three special rows explicitly.  The
       formula does not apply for n < 3. */
    if (r == 0)          /* row 0: [1] */
        return (seq_t)1;
    if (r == 1)          /* row 1: [1,1] */
        return (seq_t)1;
    if (r == 2) {        /* row 2: [1,4,1] */
        if (k == 0 || k == 2) return (seq_t)1;
        return (seq_t)4;
    }

    /* r >= 3 : use the general formula. */
    seq_t res = binom_local(r, k);

    /* 2 * C(r-2, k) */
    seq_t term = binom_local(r - 2, k);
    res += (seq_t)((unsigned __int128)2 * term);

    /* 2 * r * C(r-2, k-1)  (if k>=1) */
    if (k >= 1) {
        seq_t t = binom_local(r - 2, k - 1);
        res += (seq_t)((unsigned __int128)2 * r * t);
    }

    /* Subtract 2 only for the constant term (k==0). */
    if (k == 0) res -= (seq_t)2;

    return res;
}

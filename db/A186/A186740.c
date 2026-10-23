/*
 * A186740  Sequence read from antidiagonals of rectangular array with entry
 *          in row n and column q given by T(n,q) = 2^(2*n)*(Sum_{j=1..n+1}
 *          (cos(j*Pi/(2*q+1)))^(2*n)), n >= 0, q >= 1.
 * https://oeis.org/A186740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 79, u32 n <= 232, u64 n <= 782,
 *            u128 n <= 1275, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A186740
   Formula (sequence obtained by reading the array T(n,q) along antidiagonals):
   a(n)  =  T(n-q, q)   where n = (T(n-q, q) along anti‑diagonals)
           with  T(n,q) =  2^(2n) * Σ_{j=1}^{n+1} cos(j·π/(2q+1))^(2n)
   This implementation computes the individual array entry T(n,q) by a
   linear recurrence derived from the generating function of column q.
   The recurrence is implemented using only unsigned arithmetic;
   negative terms are handled by keeping separate positive and negative
   partial sums.  The algorithm runs in O(n*q) with n,q ≤ ~150 for any
   index up to 10 000 (well within the time limit). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* ---------- helper: triangular number (s*(s+1)/2) in 128‑bit to avoid overflow ---------- */
static inline idx_t tri(idx_t s)
{
    /* Use 128‑bit intermediate: s*(s+1) ≤ 2^128‑2^64 < 2^128  */
    __int128 x = (__int128)s * (s + 1) / 2;
    return (idx_t)x;
}

/* ---------- compute T(n,q) for given non‑negative n,q ---------- */
static seq_t compute_T(idx_t n, idx_t q)
{
    /* Allocate array for a[0..n] */
    seq_t *a = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!a) return 0;          /* if allocation fails, return 0 (should not happen) */

    /* a[0] = q (column 1 value) */
    a[0] = (seq_t)q;

    /* Pre‑compute binomial values for this q to speed up the loop */
    /* We need binomial(2q-s, s) for 1 ≤ s ≤ q (many repeated calls) */
    struct bcoeff { seq_t b; };
    struct bcoeff *B = (struct bcoeff *)malloc((size_t)q * sizeof(struct bcoeff));
    if (!B) { free(a); return 0; }

    for (idx_t s = 1; s <= q; ++s)
        B[s - 1].b = binomial(2 * q - s, s);

    for (idx_t t = 1; t <= n; ++t)
    {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;

        /* term from the numerator when t ≤ q‑1 */
        if (t <= q - 1)
        {
            idx_t coeff = q - t;                     /* (q‑t) > 0 */
            seq_t tmp  = B[t - 1].b * (seq_t)coeff;   /* binom * coeff */

            if ((t & 1) == 0)   /* (-1)^t is +1 when t even */
                sum_pos += tmp;
            else
                sum_neg += tmp;
        }

        /* recurrence part: Σ_{s=1}^{min(t,q)} (-d_s) * a[t‑s] */
        idx_t smax = t < q ? t : q;
        for (idx_t s = 1; s <= smax; ++s)
        {
            seq_t term = B[s - 1].b * a[t - s];
            if (s & 1)          /* s odd → (-d_s) > 0 → add to positive sum */
                sum_pos += term;
            else
                sum_neg += term; /* s even → (-d_s) < 0 → add to negative sum */
        }

        a[t] = sum_pos;
        a[t] -= sum_neg;                /* sum_neg ≤ sum_pos by recurrence */
    }

    seq_t result = a[n];
    free(a);
    free(B);
    return result;
}

/* ---------- main sequence function ----------------------------------------------- */
seq_t a(idx_t k)
{
    /* Find s such that tri(s) ≤ k < tri(s+1)  (s = diagonal index) */
    idx_t low = 0;
    idx_t high = 1;
    while (true)
    {
        __int128 high_tri = (__int128)high * (high + 1);
        high_tri /= 2;
        if (high_tri > k || high == (idx_t)-1) break;
        if (high == ((idx_t)-1) / 2) break;  /* avoid overflow when doubling */
        high <<= 1;
    }
    while (low + 1 < high)
    {
        idx_t mid = (low + high) / 2;
        __int128 mid_tri = (__int128)mid * (mid + 1);
        mid_tri /= 2;
        if (mid_tri <= k)
            low = mid;
        else
            high = mid;
    }
    idx_t s = low;
    idx_t pos = (idx_t)(k - tri(s));       /* position inside the diagonal (0‑based) */
    idx_t q = pos + 1;                     /* column index (starting at 1)   */
    idx_t n = s - pos;                     /* row index (starting at 0)      */

    return compute_T(n, q);
}

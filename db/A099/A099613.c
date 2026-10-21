/*
 * A099613  Numerator of (1+1/n)^k - (1+k/n), 2<=k<=n, triangle read by rows.
 * https://oeis.org/A099613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, frac
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth term of OEIS A099613.
   A099613 is the sequence of numerators of the reduced fraction

        (1 + 1/n)^k – (1 + k/n)

   where 2 ≤ k ≤ n.  The values are enumerated row‑by‑row: for n=2 we
   have the single term (k=2), then for n=3 the two terms (k=2,3),
   etc.  For the term at linear index p (1‑based) the corresponding
   (n,k) pair is obtained by solving

          (n‑1)n/2  ≥  p  ≥  (n‑2)(n‑1)/2

   so that p lies in row n and k = 1 + (p – (n‑2)(n‑1)/2).  For that
   pair the numerator equals

          (n+1)^k – n^k – k · n^(k‑1)

   The denominator is n^k, but it is never a common divisor with the
   numerator (the numerator ≡ 1 mod n), so the fraction is already in
   lowest terms and the numerator can be returned directly.               */

#include <stdint.h>

/* Compute base^exp by exponentiation by squaring, compatible with
   any unsigned seq_t (built‑in or arbitrary precision). */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1U)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

/* Find the minimal r ≥ 1 with r(r+1)/2 ≥ idx.
   The returned r is the number of terms in the previous row:
   the current row number (the n of the triangle) will be r+1. */
static idx_t row_from_index(idx_t idx)
{
    idx_t low = 1, high = 1;
    while (high * (high + 1) / 2 < idx)
        high <<= 1;                 /* exponential search for an upper bound */

    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        if (mid * (mid + 1) / 2 >= idx)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Main sequence function */
seq_t a(idx_t p)            /* p is the linear index, 1‑based */
{
    /* Determine the row (triangle n) and the position within that row. */
    idx_t r = row_from_index(p);          /* r ≥ 1; the previous row has r terms   */
    idx_t n = r + 1;                      /* current row number (triangle n)       */
    idx_t prev = r * (r - 1) / 2;          /* total terms before this row           */
    idx_t pos = p - prev;                  /* 1‑based position in this row          */
    idx_t k = pos + 1;                     /* k ranges from 2 to n                  */

    /* Compute the three powers needed. */
    seq_t m   = (seq_t)n;
    seq_t mp1 = (seq_t)(n + 1);

    seq_t pow_m_k   = pow_seq_t(m, k);
    seq_t pow_m_k1  = pow_seq_t(m, k - 1);   /* k ≥ 2, so k‑1 ≥ 1          */
    seq_t pow_mp1_k = pow_seq_t(mp1, k);

    /* Numerator: (n+1)^k – n^k – k·n^(k−1) */
    seq_t numerator = pow_mp1_k - pow_m_k;
    numerator -= (seq_t)k * pow_m_k1;

    return numerator;
}

/*
 * A193860  Triangular array: the fission of ((2x+1)^n) by (q(n,x)), where
 *          q(n,x)=x^n+x^(n-1)+...+x+1.
 * https://oeis.org/A193860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = Σ_{i=0..k} binomial(n+1, k-i) * 2^(k-i) (0 ≤ k ≤ n)
 *            a(n) is the flattening of the triangular array T(n,k) in
 *            row‑major order (row 0 first, then row 1, …)
 * Range:     u8 n <= 18, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"   /* seq_t binom(idx_t a, idx_t b); */
#include "../utils/pow2.h"    /* seq_t pow2(idx_t exp); */

seq_t a(idx_t n)
{
    /* --- locate the row r that contains index n --- */
    idx_t r = 0;
    idx_t cumulative = 1;   /* entries up to row r (inclusive) */
    while (cumulative <= n) {
        r++;                    /* move to next row */
        cumulative += (r + 1);  /* add length of new row */
    }

    /* start index of row r */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;      /* column index in row r */

    /* --- compute T(r,k) = Σ_{i=0..k} binom(r+1, k-i) * 2^(k-i) --- */
    seq_t sum = 0;
    for (idx_t i = 0; i <= k; ++i) {
        seq_t bin  = binom(r + 1, k - i);
        seq_t pow  = pow2(k - i);
        sum += bin * pow;
    }

    return sum;
}

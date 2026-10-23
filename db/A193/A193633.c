/*
 * A193633  Triangle: T(n,k)=C(4n,2k), 0<=k<=n.
 * https://oeis.org/A193633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equation: a(n) = binom(4*r , 2*c) where
 *   r = maximum integer with r*(r+1)/2 ≤ n   (row index)
 *   c = n - r*(r+1)/2                         (column index)
 *
 * This function linearises the triangle T(n,k)=C(4n,2k) row‑wise.
 */
#include <stdbool.h>
#include "../utils/seq_gcd.h"   /* seq_gcd(seq_t a, seq_t b) */

/* multiplicative binomial coefficient that works for any seq_t
 * (including 64‑bit and arbitrary‑precision). */
static seq_t my_binom(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K == 0 || K == N) return 1;
    if (K > N - K) K = N - K;  /* use symmetry */

    seq_t result = 1;
    for (idx_t i = 1; i <= K; ++i) {
        seq_t num = (seq_t)(N - K + i);   /* numerator */
        seq_t den = (seq_t)i;             /* denominator */

        /* reduce factors with each other first */
        seq_t g = seq_gcd(num, den);
        num /= g;
        den /= g;

        /* reduce denominator with current result */
        g = seq_gcd(result, den);
        result /= g;
        den /= g;

        /* multiply and divide */
        result = result * num;
        result = result / den;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* find row r and column c such that n = r*(r+1)/2 + c  with 0 ≤ c ≤ r */
    idx_t r = 0;
    idx_t prev = 0;          /* number of entries before current row */
    while (1) {
        idx_t len = r + 1;   /* entries in row r */
        if (n < prev + len)
            break;
        prev += len;
        ++r;
    }
    idx_t c = n - prev;      /* column within the row */

    /* compute the required binomial coefficient */
    return my_binom(4 * r, 2 * c);
}

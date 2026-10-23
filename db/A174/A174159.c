/*
 * A174159  Triangle read by rows. T(n, k) = 2 * Eulerian(n, k - 1) -
 *          binomial(n - 1, k - 1)* binomial(n, k - 1) / k.
 * https://oeis.org/A174159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = 2*Eulerian(n,k-1) -
 *            binomial(n-1,k-1)*binomial(n,k-1)/k. Sequence a(n) is obtained
 *            by reading the triangle row‑by‑row starting at index 1. The
 *            implementation uses only the allowed utility functions
 * Range:     u8 n <= 17, u16 n <= 39, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   binom, int_sqrt, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/int_sqrt.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

/* Return the n-th term of OEIS A174159. */
seq_t a(idx_t n)
{
    /* Find the row r such that (r-1)*r/2 < n <= r*(r+1)/2. */
    idx_t r = (int_sqrt(1 + (idx_t)8 * n) - 1) / 2;
    if ((r * (r + 1)) / 2 < n)
        r++;

    /* Position k within the row (1‑based). */
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;                 /* 1 <= k <= r */

    /* Compute Eulerian(r, k-1) by its explicit formula. */
    seq_t pos = 0;
    seq_t neg = 0;
    for (idx_t j = 0; j <= k; ++j) {
        seq_t bin = binom(r + 1, j);                     /* C(r+1, j) */
        seq_t base = (seq_t)(k - j);                     /* k-j >= 0 */
        seq_t pw   = seq_pow(base, r);                  /* (k-j)^r   */
        seq_t term = seq_mul(bin, pw);                  /* product    */
        if (j & 1)
            neg += term;
        else
            pos += term;
    }
    seq_t e = pos - neg;                                 /* Eulerian(r,k-1) */

    /* Compute the subtraction term: binom(r-1,k-1)*binom(r,k-1)/k. */
    seq_t t1 = binom(r - 1, k - 1);
    seq_t t2 = binom(r,     k - 1);
    seq_t prod = seq_mul(t1, t2);
    seq_t div  = prod / (seq_t)k;

    /* Final result: 2*e - div. */
    seq_t result = e + e;
    result -= div;
    return result;
}

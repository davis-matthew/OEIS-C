/*
 * A188644  Array of (k^n + k^(-n))/2 where k = (sqrt(x^2-1) + x)^2 for
 *          integers x >= 1.
 * https://oeis.org/A188644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the integer obtained from the square array A(n,k)
 *            defined by A(n,k) = sum_{j=0..k} C(2k,2j) * (n^2-1)^(k-j) *
 *            n^(2j), where the array is read in antidiagonal order (row
 *            i=k+offset+1, column k=s-offset-1 for index n). The sequence
 *            starts a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=7, ... (see OEI...
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/* Compute the value A(i,k) for the given row i and column k.  */
static seq_t compute_A_n_k(seq_t i, idx_t k)
{
    if (k == 0)
        return 1;

    seq_t i2minus1 = i * i - 1;
    seq_t result = 0;

    for (idx_t j = 0; j <= k; ++j)
    {
        seq_t bin   = binomial(2 * k, 2 * j);
        seq_t pw1   = big_pow(i2minus1, (seq_t)(k - j));
        seq_t pw2   = big_pow(i, (seq_t)(2 * j));
        result += bin * pw1 * pw2;
    }

    return result;
}

/* Find the antidiagonal number s (1‑based) such that the cumulative
   count of terms up to that diagonal exceeds n. */
static idx_t find_diagonal(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;

    while (high * (high + 1) / 2 <= n)
        high <<= 1;

    while (low + 1 < high)
    {
        idx_t mid = (low + high) / 2;
        if (mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid;
    }

    return high;          /* s */
}

seq_t a(idx_t n)
{
    idx_t s      = find_diagonal(n);
    idx_t prev   = (s - 1) * s / 2;
    idx_t offset = n - prev;

    idx_t i = offset + 1;          /* row index (starting at 1) */
    idx_t k = s - offset - 1;      /* column index (starting at 0) */

    return compute_A_n_k((seq_t)i, k);
}

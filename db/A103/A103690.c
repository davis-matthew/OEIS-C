/*
 * A103690  Triangle read by rows:
 *          T(n,k)=binomial(n,k-1)*k^(k-1)*(n+1-k)^(n-k) (1<=k<=n).
 * https://oeis.org/A103690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r, k-1) * k^(k-1) * (r+1-k)^(r-k) where r is the
 *            row number such that r*(r+1)/2 ≥ n, and k = n - r*(r-1)/2 is the
 *            column index
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   binom, pow_seq, row_and_col
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow_seq.h"
#include "../utils/row_and_col.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined entry, return 0 */
    idx_t r, k;
    row_and_col(n, &r, &k);

    seq_t bin = binom(r, k - 1);
    seq_t pow1 = pow_seq((seq_t)k, k - 1);
    seq_t base2 = (seq_t)(r + 1 - k);
    seq_t pow2 = pow_seq(base2, r - k);

    seq_t result = bin;
    result *= pow1;
    result *= pow2;
    return result;
}

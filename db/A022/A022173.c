/*
 * A022173  Triangle of Gaussian binomial coefficients [ n,k ] for q = 9.
 * https://oeis.org/A022173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 23, u64 n <= 57, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A022173:  Triangle of Gaussian binomial coefficients [n,k] for q = 9.
    a(n) is obtained by concatenating the rows of the triangle
    T(n,k) which satisfies
          T(0,0) = 1
          T(n,k) = T(n-1,k-1) + 9^k * T(n-1,k)   (0 ≤ k ≤ n)
    The sequence is the list of T(n,k) for n = 0,1,2,… and k = 0,…,n.  */

#include <stdint.h>          /* for uint64_t */

seq_t a(idx_t n)
{
    /* Find the row r such that n lies in row r.  */
    idx_t r = 0;
    while (/* compute C(r) = (r+1)(r+2)/2 – 1 */ 1 &&
           n > ((r + 1) * (r + 2)) / 2 - 1) {
        ++r;
    }

    /* k is the position within the row (0‑based).  */
    idx_t first = r * (r + 1) / 2;       /* first index of row r   */
    idx_t k     = n - first;             /* 0 ≤ k ≤ r              */

    /* Edge cases.  */
    if (k == 0 || k == r)
        return (seq_t)1;

    /* Pre‑compute powers of 9 up to r (q = 9).  */
    static seq_t pow9[200];
    pow9[0] = (seq_t)1;
    for (idx_t i = 1; i <= r; ++i)
        pow9[i] = pow9[i - 1] * (seq_t)9;

    /* Compute Gaussian binomial [r,k]_9 multiplicatively.  */
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t numerator   = pow9[r - i + 1] - (seq_t)1;  /* 9^{r-i+1} – 1 */
        seq_t denominator = pow9[i]     - (seq_t)1;      /* 9^{i}   – 1 */
        result = (result * numerator) / denominator;
    }
    return result;
}

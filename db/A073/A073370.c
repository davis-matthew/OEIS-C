/*
 * A073370  Convolution triangle of A001045(n+1) (generalized
 *          (1,2)-Fibonacci), n>=0.
 * https://oeis.org/A073370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 93, u32 n <= 328, u64 n <= 1138,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial, floor_sqrt, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A073370 – convolution triangle T(n,m).
    a(n) is the n‑th entry of the triangle read row by row:
         a(0) = T(0,0)
         a(1) = T(1,0), a(2) = T(1,1)
         a(3) = T(2,0), a(4) = T(2,1), …
    Formula for T(r,c):
        T(r,c) = ∑_{k=0..⌊(r-c)/2⌋}  binomial(r-k, c) *
                                 binomial(r-c-k, k) * 2^k
    (binomial, pow2, floor_sqrt are provided in the build.)
    The function works for any seq_t and idx_t, does not use
    floating point, and can be called in arbitrary order.        */

#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/floor_sqrt.h"

/* Return the a(n) term of sequence A073370. */
seq_t a(idx_t n)
{
    /* Compute the row index r from the linear index n.
       We use the integer square root of 1+8n.
       Let t = 1 + 8n,  r = floor((sqrt(t)-1)/2).                */
    seq_t t = (seq_t)n * 8 + 1;
    seq_t sqrt_t = floor_sqrt(t);
    idx_t r = (idx_t)((sqrt_t - 1) / 2);

    /* cumulative number of terms before row r : r*(r+1)/2   */
    idx_t prev = (idx_t)((seq_t)r * (r + 1) / 2);

    /* column index c within the row  */
    idx_t c = n - prev;      /* 0 ≤ c ≤ r */

    /* Compute T(r,c) = Σ_{k=0..⌊(r-c)/2⌋} binomial(r-k,c)*binomial(r-c-k,k)*2^k */
    seq_t sum = 0;
    idx_t kmax = (r - c) / 2;
    for (idx_t k = 0; k <= kmax; ++k)
    {
        seq_t term = binomial(r - k, c);
        term *= binomial(r - c - k, k);
        term *= pow2(k);
        sum += term;
    }
    return sum;
}

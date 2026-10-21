/*
 * A218695  Square array A(h,k) = (2^h-1)*A(h,k-1) + Sum_{i=1..h-1}
 *          binomial(h,h-i)*2^i*A(i,k-1), with A(1,k) = A(h,1) = 1; read by
 *          antidiagonals.
 * https://oeis.org/A218695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 30, u32 n <= 59, u64 n <= 125,
 *            u128 n <= 240, big any n
 * Helpers:   big_pow, binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A218695: Square array A(h,k) = (2^h-1)*A(h,k-1) + ∑_{i=1}^{h-1} C(h,h-i)*2^i*A(i,k-1),
   with A(1,k)=A(h,1)=1; read by anti‑diagonals.
   The sequence a(n) is obtained by traversing the array anti‑diagonally
   and listing the entries.  The index n starts at 1.
   Formula used: A(h,k) = ∑_{i=0}^{h} (-1)^{h-i} * C(h,i) * (2^i-1)^k
*/

#include "../utils/pow2.h"      /* pow2(idx_t exp) -> seq_t */
#include "../utils/big_pow.h"   /* big_pow(seq_t base, seq_t exp) -> seq_t */
#include "../utils/binomial.h"  /* binomial(idx_t n, idx_t k) -> seq_t */

/* Compute a single table entry A(h,k) using the explicit formula */ 
static seq_t A_h_k(idx_t h, idx_t k)
{
    seq_t pos = 0;   /* sum of terms with (-1)^{h-i} = +1 */
    seq_t neg = 0;   /* sum of terms with (-1)^{h-i} = -1 */
    for (idx_t i = 0; i <= h; ++i) {
        seq_t base   = pow2(i) - (seq_t)1;          /* (2^i-1) */
        seq_t power  = big_pow(base, (seq_t)k);     /* (2^i-1)^k   */
        seq_t binom  = binomial(h, i);             /* C(h,i)      */
        seq_t term   = binom * power;              /* C(h,i)*(2^i-1)^k */
        if (((h - i) & 1U) == 0) {                 /* even parity → + */
            pos += term;
        } else {                                   /* odd  parity → - */
            neg += term;
        }
    }
    /* pos ≥ neg, because A(h,k) is non‑negative */
    return pos - neg;
}

/* Convert a single OEIS index n (1‑based) into the corresponding (h,k)
   entry of the array and return its value. */
seq_t a(idx_t n)
{
    if (n == 0)               /* safety; the sequence is offset 1 */
        return (seq_t)0;

    /* Find the diagonal number t: minimal t with t*(t+1)/2 ≥ n */
    idx_t t = 1;
    while ((t * (t + 1U)) / 2U < n)
        ++t;

    /* Number of entries before diagonal t */
    idx_t before = ((t - 1U) * t) / 2U;

    /* Zero‑based offset inside diagonal t */
    idx_t offset = n - before - 1U;

    /* Convert to (h,k).  Diagonal t contains pairs (h,k) with
       h = 1..t  and  k = t+1-h. */
    idx_t h = offset + 1U;
    idx_t k = t + 1U - h;

    return A_h_k(h, k);
}

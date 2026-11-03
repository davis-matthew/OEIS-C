/*
 * A181432  Symmetric square array T(n,k) read by antidiagonals.
 *          T(n,k)=A008836(n)*A008836(k).
 * https://oeis.org/A181432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 83, i16 n <= 83, i32 n <= 83, i64 n <= 83, i128 n <= 83,
 *            big any n
 * Validated: partial, pass, 83 terms through n = 83 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A181432:  a(n) = λ(i) * λ(j) where λ(k) is Liouville's function
   (λ(k) = (-1)^{Ω(k)} with Ω(k) the total number of prime factors of k,
   counted with multiplicity).  
   (i,j) are the indices on the antidiagonal that contains the n‑th term:
   t_{d-1} < n ≤ t_d  with t_d = d*(d+1)/2, and
   i = n - t_{d-1} ,  j = d + 1 - i.  */

#include <stdint.h>
#include <stdbool.h>

/* Liouville's function λ(k) = (-1)^{Ω(k)} */
static inline seq_t liouville(idx_t k)
{
    if (k <= 1)
        return (seq_t)1;          /* λ(1) = 1 */
    int parity = 0;               /* parity of Ω(k) */
    idx_t m = k;
    for (idx_t p = 2; p * p <= m; ++p) {
        while (m % p == 0) {
            parity ^= 1;          /* toggle parity for each prime factor */
            m /= p;
        }
    }
    if (m > 1)                    /* remaining prime factor */
        parity ^= 1;
    return parity ? (seq_t)-1 : (seq_t)1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* out of bounds, will never happen with offset 1 */

    /* Find diagonal d such that t_{d-1} < n ≤ t_d */
    idx_t d = 1;
    while ( ((unsigned __int128)d * (d + 1) / 2) < n )
        ++d;

    idx_t t_prev = (d - 1) * d / 2;      /* t_{d-1} */
    idx_t i = n - t_prev;                /* row index (1‑based) */
    idx_t j = d + 1 - i;                 /* column index (1‑based) */

    seq_t li = liouville(i);
    seq_t lj = liouville(j);
    return li * lj;
}

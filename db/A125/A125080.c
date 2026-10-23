/*
 * A125080  Triangle, read by rows, defined by T(n,k) =
 *          A000108(n-k)*A001147(k)*C(n,2*k), for k=0..[n/2], n>=0, where
 *          A000108 is the Catalan numbers and A001147 is the double
 *          factorials.
 * https://oeis.org/A125080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n,k) = A000108(n-k) * A001147(k) * C(n,2*k)
 * Range:     u8 n <= 11, u16 n <= 26, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(idx_t n) returns the n-th entry of the triangle when it is
   read row‑by‑row (a(0)=1, a(1)=1, a(2)=2, a(3)=1, …).          */

#include <stdbool.h>
#include "../utils/catalan.h"
#include "../utils/binomial.h"

/* compute the double factorial (2k-1)!! for k ≥ 0 */
static seq_t double_factorial(idx_t k)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * ((seq_t)2 * i - 1);
    return res;
}

seq_t a(idx_t n)
{
    /* find the row 'row' and column 'k' that correspond to flat index n */
    idx_t row = 0;
    idx_t pos = n;
    while (pos >= ((row / 2) + 1)) {
        pos -= ((row / 2) + 1);
        ++row;
    }
    idx_t k = pos;                /* 0 ≤ k ≤ floor(row/2) */

    seq_t cat = catalan(row - k);          /* A000108 */
    seq_t df  = double_factorial(k);       /* A001147 */
    idx_t doublek = k << 1;                /* 2*k */
    seq_t bino = binomial(row, doublek);   /* C(row,2k) */

    return cat * df * bino;
}

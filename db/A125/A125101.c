/*
 * A125101  T(n,k) = k*binomial(n-1,k-1) + Fibonacci(k)*binomial(n-1,k) (1 <=
 *          k <= n).
 * https://oeis.org/A125101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A125101
   a(n) = T(r,k) where T(n,k)=k*binom(n-1,k-1)+Fib(k)*binom(n-1,k)
   The triangle is read row‑by‑row; row r contains r terms.  The
   integer r that satisfies r*(r+1)/2 ≥ n gives the row and
   k = n - r*(r-1)/2 is the column. */

#include "../utils/binom.h"
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* find the row r (1‑based) containing position n */
    idx_t r = 1;
    while (((seq_t)r * ((seq_t)r + 1u)) / 2u < n)
        ++r;

    /* elements preceding this row */
    idx_t prev = (r - 1u) * r / 2u;

    /* column k within the row (1‑based) */
    idx_t k = n - prev;

    seq_t bin1 = binom(r - 1u, k - 1u);   /* C(r-1, k-1) */
    seq_t bin2 = binom(r - 1u, k);        /* C(r-1, k)   */
    seq_t fibk = fib(k);                  /* Fibonacci(k) */

    return (seq_t)k * bin1 + fibk * bin2;
}

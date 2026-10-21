/*
 * A213566  Rectangular array: (row n) = b**c, where b(h) = F(h), c(h) =
 *          (n-1+h)^2, F = A000045 (Fibonacci numbers), n>=1, h>=1, and ** =
 *          convolution.
 * https://oeis.org/A213566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 153, u32 n <= 820, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A213566:  read by falling antidiagonals the array
 * T(n,k) with
 *   T(n,k) = n*(n*F(k+2)+2*F(k+3)) + F(k+6)
 *            - (n+2)*(2*k+n+2) - k^2 - 4
 *
 * F(m) is the m‑th Fibonacci number (F(0)=0,F(1)=1).
 * The sequence a(n) lists T(i,j) in the order
 *   s = i+j increasing, with (i,j) = (1,s-1),(2,s-2),…,(s-1,1).
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* offset guard – a(0) is undefined */

    /* Locate the antidiagonal (s = i + j) that contains entry n. */
    idx_t s   = 2;          /* current diagonal sum (i+j) */
    idx_t cum = 0;          /* number of entries before this diagonal */

    while (cum + (s - 1) < n) {
        cum += (s - 1);
        ++s;
    }

    /* Position inside the diagonal: 0‑based. */
    idx_t pos = n - cum - 1;
    idx_t i   = pos + 1;        /* row index, 1‑based */
    idx_t j   = s - i;          /* column index, 1‑based */

    /* Compute T(i,j) using the closed form. */
    seq_t row = (seq_t)i;
    seq_t col = (seq_t)j;

    seq_t Fk2 = fib(j + 2);
    seq_t Fk3 = fib(j + 3);
    seq_t Fk6 = fib(j + 6);

    seq_t term_left = row * (row * Fk2 + ((seq_t)2u) * Fk3);
    seq_t result    = term_left + Fk6;

    seq_t sub = ((seq_t)(i + 2)) * (
                    ((seq_t)2u * col) + row + ((seq_t)2u)
                );

    result -= sub;
    result -= col * col;
    result -= ((seq_t)4u);

    return result;
}

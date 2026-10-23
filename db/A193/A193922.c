/*
 * A193922  Mirror of the triangle A193921.
 * https://oeis.org/A193922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   For n ≥ 0 let r be the unique integer with r·(r+1)/2 ≤ n <
 *            (r+1)·(r+2)/2 (i.e., r is the row containing the n‑th term when
 *            the triangle is read by rows). Let k = n – r·(r+1)/2. Then a(0)
 *            = 1 a(n) = Fib(r+2) – Fib(k+1) (n > 0) where Fib(m) is the m-th
 *            Fibonacci number (Fib(0)=0, Fib(1)=1). The tri...
 * Range:     u8 n <= 77, u16 n <= 275, u32 n <= 1080, u64 n <= 4277,
 *            u128 n <= 9830, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* find the row r containing the index n */
    idx_t r = 0;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* compute position k within that row (0‑based) */
    idx_t before = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - before;

    if (r == 0)          /* the single element 1 in the first row */
        return (seq_t)1;

    seq_t fr2 = fib(r + 2);   /* Fibonacci(r+2) */
    seq_t fk1 = fib(k + 1);   /* Fibonacci(k+1) */
    return fr2 - fk1;
}

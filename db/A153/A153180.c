/*
 * A153180  a(n) = L(13n)/L(n) where L(n) = Lucas number A000204(n).
 * https://oeis.org/A153180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = L(13*n)/L(n) (Lucas numbers)
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/fib_pair.h"

/* Return Lucas number L(k) using fast Fibonacci pair */
static seq_t lucas_of(idx_t k)
{
    seq_t f, f1;
    fib_pair(k, &f, &f1);          /* f  = F(k),   f1 = F(k+1) */
    /* L(k) = 2*F(k+1) - F(k)  */
    seq_t twice = f1 + f1;
    twice -= f;
    return twice;
}

/* a(n) = L(13*n) / L(n)  for n >= 1 (undefined for n == 0) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined only for n >= 1 */
    seq_t L13 = lucas_of(n * (idx_t)13);
    seq_t Ln  = lucas_of(n);
    return L13 / Ln;
}

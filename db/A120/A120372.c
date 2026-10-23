/*
 * A120372  a(n) = ((2n)!+1)*((2n+2)!+1) mod (2n+1)*(2n+3).
 * https://oeis.org/A120372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2n)!+1)*((2n+2)!+1) mod (2n+1)*(2n+3)
 * Range:     u8 n <= 8, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is 1‑based; behaviour for n<1 is undefined but we guard anyway */
    if (n < 1)
        return 0;

    /* denominator: (2n+1)*(2n+3) */
    seq_t denom = (seq_t)(2 * n + 1) * (seq_t)(2 * n + 3);

    /* factorial of (2n) modulo denom */
    seq_t fact = 1;
    for (idx_t i = 2; i <= 2 * n; ++i)
        fact = (fact * (seq_t)i) % denom;

    /* (2n)!  */
    seq_t f1 = fact;

    /* (2n+2)! = (2n)! * (2n+1) * (2n+2)  modulo denom */
    seq_t f2 = (f1 * (seq_t)(2 * n + 1)) % denom;
    f2 = (f2 * (seq_t)(2 * n + 2)) % denom;

    /* compute the required product modulo denom */
    seq_t t1 = (f1 + 1) % denom;
    seq_t t2 = (f2 + 1) % denom;
    return (t1 * t2) % denom;
}

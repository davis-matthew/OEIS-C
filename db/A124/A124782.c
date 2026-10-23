/*
 * A124782  a(n) = (n+3)/gcd(A(n), A(n+2)) where A(n) = A000522(n) =
 *          Sum_{k=0..n} n!/k!.
 * https://oeis.org/A124782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+3)/gcd(A000522(n), A000522(n+2))
 * Range:     u8 n <= 79, u16 n <= 4096, u32 n <= 4096, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* with A000522( n ) = Sum_{k=0..n} n!/k! */
/* The identity A(n+2) = (n+2)(n+1)·A(n) + n+3 reduces the gcd to gcd(A(n), n+3). */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* m = n + 3 : the divisor in the definition of a(n) */
    const seq_t m = (seq_t)n + 3;

    /* Compute A(n) modulo m using the recurrence
       A(0) = 1
       A(k) = k * A(k-1) + 1, for k >= 1   */
    seq_t A_mod = 1 % m;   /* A(0) mod m */
    for (idx_t k = 1; k <= n; ++k)
        A_mod = ((seq_t)k * A_mod + 1) % m;

    /* gcd(A(n),n+3) == gcd(A_mod, m) */
    const seq_t g = gcd(A_mod, m);

    /* Final value of a(n) */
    return m / g;
}

/*
 * A186423  Partial sums of A186421.
 * https://oeis.org/A186423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n*(n+1) + 3 + (-1)^n*(2*n+1) - 4*(-1)^{⌊(n+1)/2⌋}) /
 *            16
 * Range:     u8 n <= 25, u16 n <= 417, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A186423 (partial sums of A186421).  */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use an intermediate signed type big enough for all operations. */
    __int128 nn = ( __int128 ) n;

    /* Base term: 6*n*(n+1) + 3 */
    __int128 term = ( (__int128)6 * nn * (nn + 1) ) + 3;

    /* Add or subtract (2*n+1) depending on the parity of n. */
    if (n & 1)                 /* n odd */
        term -= ( (__int128)2 * nn + 1 );
    else                       /* n even */
        term += ( (__int128)2 * nn + 1 );

    /* Compute the parity of floor((n+1)/2). */
    unsigned parity = ( ( (n + 1) >> 1 ) & 1 );
    /* If parity is odd, (-1)^{floor((n+1)/2)} = -1, so add 4.
       Otherwise add -4. */
    if (parity)
        term += 4;            /* subtracting -4 */
    else
        term -= 4;            /* subtracting +4 */

    /* Divide by 16 and cast back to the target type. */
    return (seq_t)(term / 16);
}

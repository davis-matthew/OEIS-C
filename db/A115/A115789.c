/*
 * A115789  a(n) = (floor((n+1)*Pi) - floor(n*Pi)) mod 2.
 * https://oeis.org/A115789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = (floor((n+1)*Pi) - floor(n*Pi)) mod 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>      /* floorl, acosl */

seq_t a(idx_t n)
{
    /* Constants: long double PI offers high precision for n up to ~1e19. */
    static const long double PI = acosl(-1.0L);

    /* Compute the floor of (n+1)*π and n*π using long double arithmetic. */
    long double f1 = floorl((long double)(n + 1) * PI);
    long double f0 = floorl((long double)n * PI);

    /* Difference between successive floors is either 3 or 4. */
    long double diff = f1 - f0;

    /* Return the parity of the difference. */
    return (seq_t)((unsigned long long)diff & 1);
}

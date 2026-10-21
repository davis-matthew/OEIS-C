/*
 * A095263  a(n+3) = 3*a(n+2) - 2*a(n+1) + a(n), starting with (1, 3, 7).
 * https://oeis.org/A095263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Padovan(3n) = a(n) a(n+3) = 3*a(n+2) - 2*a(n+1) + a(n), a(1)=1,
 *            a(2)=3, a(3)=7 The Padovan sequence P satisfies:
 *            P(1)=P(2)=P(3)=1, P(k)=P(k-2)+P(k-3) for k≥4. Here a(n)=P(3n).
 *            Hence the values are obtained by computing Padovan(3*n)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 53, u128 n <= 105,
 *            big any n
 * Helpers:   padovan_at
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/padovan_at.h"

seq_t a(idx_t n)
{
    /* offset 1: a(1) is the first term */
    if (n == 0)
        return (seq_t)0; /* undefined, but return 0 for safety */
    /* compute Padovan(3n) */
    return padovan_at(3 * n);
}

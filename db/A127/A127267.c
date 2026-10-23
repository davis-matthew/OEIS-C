/*
 * A127267  a(n) = floor(n/pi(n)), where pi(n)=A000720(n) is the number of
 *          primes <=n.
 * https://oeis.org/A127267
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n / pi(n)) where pi(n) = A000720(n)
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n < 2)   /* behaviour for indices < 2 is not required */
        return 0;
    seq_t pi = compute_pi(n);           /* pi(n)  */
    /* pi(n) >= 1 for n >= 2, so the division is safe */
    return (seq_t)n / pi;
}

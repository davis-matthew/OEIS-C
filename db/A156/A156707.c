/*
 * A156707  For all numbers k(n) congruent to +1 or -1 (mod 4) starting with
 *          k(n) = {3,5,7,9,11,...}, a(k(n)) is the congruence (mod 4) if k(n)
 *          is prime and 0 if k(n) is composite.
 * https://oeis.org/A156707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = prime(k(n)) ? ((k(n) mod 4 == 1) ? 1 : -1) : 0, where
 *            k(n) = 4*ceil(n/2) + (-1)^n (i.e., k(n) = 4*((n+1)/2) ± 1)
 * Range:     i8 n <= 9539, i16 n <= 9539, i32 n <= 9539, i64 n <= 9539,
 *            i128 n <= 9539, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* k(n) = 4*ceil(n/2) + (-1)^n */
    idx_t k = 4 * ((n + 1) / 2);
    if (n & 1)          /* n is odd → subtract 1 */
        k -= 1;
    else                /* n is even → add 1 */
        k += 1;

    if (!is_prime(k))
        return 0;

    return (seq_t)(k % 4 == 1 ? 1 : -1);
}

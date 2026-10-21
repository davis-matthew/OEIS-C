/*
 * A089682  Primes of the form 3*m^2 - 1.
 * https://oeis.org/A089682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = primes of the form 3*m^2 - 1 where m = 1 or even (m ≥ 1)
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 34, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* Offset is 1; return 0 for n == 0 as a safe sentinel */
        return (seq_t)0;

    idx_t  found = 0;
    seq_t  m     = 1;           /* first m gives p = 3*1^2 - 1 = 2 */

    for (;;) {
        seq_t cand = (seq_t)3 * m * m - 1;   /* candidate prime */

        if (cand >= (seq_t)2 && is_prime_big(cand)) {
            ++found;
            if (found == n)
                return cand;
        }

        /* next m: after 1 we must use even m values */
        if (m == 1)
            m = 2;
        else
            m += 2;
    }
}

/*
 * A125164  Positive integers n such that no prime exists of the form (k! + 3n
 *          - 1), (k! + 3n) or (k! + 3n + 1) for any k.
 * https://oeis.org/A125164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth odd integer >1 for which 3*a(n)+2 is composite
 * Range:     u8 n <= 15, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* offset 1 – not used */

    idx_t count     = 0;
    idx_t candidate = 3;                       /* start with the first odd > 1 */

    while (1) {
        seq_t test = (seq_t)3 * (seq_t)candidate + 2;
        if (!is_prime_big(test)) {              /* 3*candidate+2 is composite */
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate += 2;                         /* next odd integer */
    }
}

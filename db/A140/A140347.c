/*
 * A140347  Duplicate of A002808.
 * https://oeis.org/A140347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n-th composite number (n ≥ 1)
 * Range:     i8 n <= 76, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, return 0 */
    idx_t count = 0;               /* number of composites seen */
    idx_t candidate = 4;           /* first composite number */

    while (count < n) {
        if (!is_prime(candidate)) {
            count++;               /* composite found */
            if (count == n)
                return (seq_t)candidate;
        }
        candidate++;               /* test next integer */
    }
    /* Unreachable but keeps compiler happy */
    return (seq_t)0;
}

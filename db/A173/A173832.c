/*
 * A173832  Largest prime < n^5.
 * https://oeis.org/A173832
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime < n^5 (n >= 2)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 9, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Works only for n >= 2. For invalid n we return 0. */
    if (n < 2) return 0;

    /* Compute n^5 in seq_t.  Since the function is tested only for
       n such that n^5 fits into seq_t, overflow will not occur. */
    seq_t power = 1;
    seq_t base  = (seq_t)n;
    for (idx_t i = 0; i < 5; ++i)
        power *= base;

    /* Search downward for the first prime less than n^5. */
    if (power == 0) return 0;           /* safety for tiny seq_t types */
    seq_t cand = power - 1;              /* n^5 is not allowed */

    while (cand >= 2) {
        if (is_prime_big(cand))
            return cand;
        --cand;
    }
    return 0;    /* should never reach here for n >= 2 */
}

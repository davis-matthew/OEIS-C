/*
 * A192992  Odd n > 1 such that n+1 is a semiprime times a power of two.
 * https://oeis.org/A192992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth odd integer >1 for which n+1 is a semiprime
 *            times a power of two. Computed via incremental search
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_semiprime_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_semiprime_ull.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n < 1, harmless */

    uint64_t found = 0;                  /* how many terms found so far */
    uint64_t candidate = 3ULL;           /* odd candidates > 1 */

    for (;;) {
        /* n+1 is at least 4, so it has at least one factor of 2.
           Divide by 2 repeatedly and check after each division
           whether the quotient is a semiprime. */
        uint64_t temp = candidate + 1ULL;

        while ((temp & 1ULL) == 0ULL) {  /* temp is divisible by 2 */
            temp >>= 1UL;                /* divide by 2 */
            if (is_semiprime_ull(temp)) {
                ++found;
                if (found == n) {
                    return (seq_t)candidate;
                }
            }
        }
        candidate += 2ULL;                /* next odd integer */
    }
}

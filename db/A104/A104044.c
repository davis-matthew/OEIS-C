/*
 * A104044  Numbers k such that 10*k + 7 is prime and k is a multiple of ten.
 * https://oeis.org/A104044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 10*m where m is the (n‑1)-th non‑negative integer such that 100*m+7 is prime */
/* k is a multiple of 10 and 10*k+7 is prime */

#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    for (uint64_t m = 0; ; ++m) {
        uint64_t p = 100ULL * m + 7ULL;   /* candidate prime 10*(10*m)+7 */
        if (is_prime(p)) {
            ++found;
            if (found == n) {
                return (seq_t)(10ULL * m);   /* k = 10*m */
            }
        }
    }
}

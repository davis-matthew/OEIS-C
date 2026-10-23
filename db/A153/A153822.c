/*
 * A153822  Numbers 41*k such that 41*k+2 and 41*k-6 are both prime.
 * https://oeis.org/A153822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 14, u32 n <= 21, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   bad_residue, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: numbers 41*k such that 41*k+2 and 41*k-6 are both prime
 * (OEIS A153822)
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/bad_residue.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n = 0 */

    idx_t found = 0;
    idx_t k     = 0;        /* will be incremented before test */

    while (found < n) {
        ++k;                     /* candidate k */

        if (bad_residue(k))
            continue;            /* skip most candidates quickly */

        seq_t val = (seq_t)41 * (seq_t)k;   /* a = 41*k   */
        seq_t p1  = val + 2;
        seq_t p2  = val - 6;

        if (is_prime_big(p1) && is_prime_big(p2)) {
            ++found;
            if (found == n)
                return val;
        }
    }

    /* unreachable: protects against empty return */
    return 0;
}

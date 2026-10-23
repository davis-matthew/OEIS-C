/*
 * A102342  Numbers k such that 10k + 7 is prime.
 * https://oeis.org/A102342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that 10*k + 7 is prime */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* 0‑based internal count of found k */
    idx_t found = 0;
    seq_t k = 0;

    while (found < n) {
        /* Test if 10*k + 7 is prime */
        uint64_t candidate = (uint64_t)10 * (uint64_t)k + 7;
        if (is_prime(candidate)) {
            found++;
            if (found == n)
                return k;
        }
        k++;
    }
    /* unreachable, but keep compiler happy */
    return k;
}

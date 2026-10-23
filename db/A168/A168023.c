/*
 * A168023  Noncomposite numbers in the northern ray of the Ulam spiral as
 *          oriented on the March 1964 cover of Scientific American.
 * https://oeis.org/A168023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4k^2 - 9k + 6 for k = 1,2,3,… yielding 1 or a prime. The
 *            function scans k and collects such values until the nth one is
 *            found
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 23, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        __int128 t = (__int128)4 * k * k - (__int128)9 * k + 6;
        if (t <= 0) continue;                  /* safety for very small k */
        seq_t val = (seq_t)t;                   /* works for all seq_t sizes */
        if (val == 1 || is_prime_big(val)) {
            ++count;
            if (count == n)
                return val;
        }
    }
}

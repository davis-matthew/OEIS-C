/*
 * A179329  Number of iterations of (n + lpf(n)) / 2 required to reach a
 *          prime, where lpf equals the least prime factor.
 * https://oeis.org/A179329
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of times needed to apply f(x) = (x + lpf(x))/2
 *            until a prime is reached (with lpf = least prime factor)
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   is_prime, lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/lpf.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t cur = (seq_t)n;
    /* Repeatedly apply f until cur becomes prime. */
    /* Use is_prime on the value cast to idx_t (well within limits). */
    while (!is_prime((idx_t)cur)) {
        /* lpf is defined on idx_t; cast cur appropriately. */
        seq_t l = (seq_t)lpf((idx_t)cur);
        cur = (cur + l) / 2;
        count++;
    }
    return count;
}

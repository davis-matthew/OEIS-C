/*
 * A105161  Difference between n and the second-smallest prime larger than n.
 * https://oeis.org/A105161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(pi(n)+2) - n
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t cur = n + 1;
    idx_t p1 = 0;
    /* find the first prime greater than n */
    while (true) {
        if (is_prime(cur)) { p1 = cur; break; }
        cur++;
    }
    /* find the second prime greater than n */
    cur = p1 + 1;
    idx_t p2 = 0;
    while (true) {
        if (is_prime(cur)) { p2 = cur; break; }
        cur++;
    }
    return (seq_t)(p2 - n);
}

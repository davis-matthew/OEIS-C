/*
 * A002330  Value of y in the solution to p = x^2 + y^2, x <= y, with prime p
 *          = A002313(n).
 * https://oeis.org/A002330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search — find p = A002313(n) (p = 2 or prime p == 1 mod 4), then
 *            search for x <= y with p = x^2 + y^2; a(n) = y
 * Range:     u8 n <= 3258, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a002330_isprime, a002330_nth_p
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a002330_isprime.h"
#include "../utils/a002330_nth_p.h"

seq_t a(idx_t n) {
    uint64_t p = a002330_nth_p(n);
    uint64_t y = 0;
    while ((y + 1) * (y + 1) <= p) y++;
    uint64_t x = 1;
    while (x <= y) {
        uint64_t s = x * x + y * y;
        if (s == p) return (seq_t)y;
        if (s < p) x++;
        else y--;
    }
    return (seq_t)0;
}

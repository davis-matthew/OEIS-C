/*
 * A085654  Duplicate of A074719.
 * https://oeis.org/A085654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = pi(reverse(n)) (reverse of n in base 10); duplicate of
 *            A074719
 * Range:     i8 n <= 28, i16 n <= 28, i32 n <= 28, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 28 (2026-11-07)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    idx_t rev = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        rev = rev * 10 + (tmp % 10);
        tmp /= 10;
    }
    return prime_pi(rev);
}

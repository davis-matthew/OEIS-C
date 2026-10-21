/*
 * A079042  Numbers n in which the first k digits of n form an integer
 *          divisible by k^2, for k = 1, 2, ..., m, where m is the number of
 *          digits in n.
 * https://oeis.org/A079042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   is_good
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A079042.  
   A(n) is the nth positive integer such that for every k = 1..m
   (m = number of decimal digits of the integer), the first k digits
   form a number divisible by k². */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_good.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1, but harmless */

    seq_t candidate = 0;
    idx_t found = 0;

    while (found < n) {
        ++candidate;
        if (is_good(candidate)) ++found;
    }

    return candidate;
}

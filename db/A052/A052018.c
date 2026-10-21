/*
 * A052018  Numbers k with the property that the sum of the digits of k is a
 *          substring of k.
 * https://oeis.org/A052018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 3317, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_sum_substring
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that the sum of the digits of k is a decimal
   substring of the decimal representation of k.  The function
   enumerates integers and returns the n-th such number. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_sum_substring.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t k = 0;

    while (true) {
        if (has_sum_substring(k)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}

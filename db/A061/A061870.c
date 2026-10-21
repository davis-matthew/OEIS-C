/*
 * A061870  Numbers such that |first digit - second digit + third digit -
 *          fourth digit ...| = 1.
 * https://oeis.org/A061870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 7370, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alt_sum_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers such that |first digit - second digit + third digit - fourth digit ...| = 1
   The implementation scans all integers in increasing order and selects those
   whose alternating digit sum equals ±1. */

#include <stdint.h>
#include <limits.h>
#include "../utils/alt_sum_digits.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t candidate = 1;

    while (1) {
        int s = alt_sum_digits(candidate);
        if (s == 1 || s == -1) {
            ++count;
            if (count == n) {
                return candidate;
            }
        }
        ++candidate;
    }
}

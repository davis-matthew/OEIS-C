/*
 * A225017  Odd part of digit sum of 5^n divided by maximal possible power of
 *          5.
 * https://oeis.org/A225017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = odd_part( digit_sum(5^n) / 5^ν_5(digit_sum(5^n)) )
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum_of_power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  odd_part removes all factors of 2 from the integer */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum_of_power5.h"

seq_t a(idx_t n)
{
    uint64_t s = digit_sum_of_power5(n);

    /* Remove all factors of 5. */
    while (s % 5 == 0) {
        s /= 5;
    }

    /* Remove all factors of 2 (odd part). */
    while (s % 2 == 0) {
        s /= 2;
    }

    return (seq_t)s;
}

/*
 * A027870  Number of zero digits in 2^n.
 * https://oeis.org/A027870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the number of zero digits in the decimal representation
 *            of 2^n
 * Range:     u8 n <= 7359, u16 n <= 9912, u32 n <= 9912, u64 n <= 9912,
 *            u128 n <= 9912, big any n
 * Helpers:   count_zero_digits_of_2_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: construct 2^n in base‑10 using an array of digits, multiply by 2 n times,
   and count the occurrences of 0. This works for any seq_t, including 64‑bit or arbitrary‑precision types. */

#include <stdlib.h>
#include "../utils/count_zero_digits_of_2_pow.h"

seq_t a(idx_t n)
{
    return count_zero_digits_of_2_pow(n);
}

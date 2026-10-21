/*
 * A075826  n minus (number of letters in English name of n).
 * https://oeis.org/A075826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, word, less
 * Formula:   a(n) = n - number_of_letters_in_english_name(n)
 * Range:     i8 n <= 70, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   letter_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/letter_count_u64.h"

extern seq_t a(idx_t n)
{
    return (seq_t)n - letter_count_u64(n);
}

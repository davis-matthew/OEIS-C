/*
 * A067114  Let N = 24681012141618202224262830..., the concatenation of the
 *          even numbers. Then a(n) = sum of first n digits of N.
 * https://oeis.org/A067114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   digits_count, sum_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: cumulative sum of digits of concatenated even numbers.
   a(n) = sum of the first n decimal digits of the infinite string
   N = 2468101214161820...
   Implementation: iterate through the even numbers, summing the
   digits of each until the requested digit count is reached;
   if the last number is only partly used, sum only its leading
   digits.  This runs in O(k) where k is the number of even
   numbers needed (roughly n / average_digit_count). */

#include <stdint.h>
#include "../utils/digits_count.h"
#include "../utils/sum_digits.h"

static unsigned long long pow10[20] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL
};

static unsigned int sum_first_digits(unsigned long long number,
                                     int total_digits,
                                     int count) {
    unsigned int sum = 0;
    for (int i = 0; i < count; ++i) {
        int exp = total_digits - i - 1;          /* positional power of 10 */
        unsigned long long digit = (number / pow10[exp]) % 10;
        sum += (unsigned int)digit;
    }
    return sum;
}

/* External linkage as required */
seq_t a(idx_t n) {
    seq_t total = 0;
    unsigned long long current = 2ULL; /* first even number */
    idx_t remaining = n;              /* digits still needed */

    while (remaining > 0) {
        int d = digits_count(current); /* digits in current number */

        if ((idx_t)d <= remaining) {
            /* whole number contributes */
            total += (seq_t)sum_digits(current);
            remaining -= (idx_t)d;
            current += 2ULL;           /* move to next even */
        } else {
            /* only a prefix of this number is needed */
            total += (seq_t)sum_first_digits(current, d, (int)remaining);
            break;                     /* done */
        }
    }
    return total;
}

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
 * Validated: standard, pass, 81 terms through n = 9500 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  odd_part removes all factors of 2 from the integer */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper: compute the digit sum of 5^n using decimal big integer arithmetic. */
static uint64_t digit_sum_of_power5(idx_t n)
{
    /* Use base 1e9 to hold the decimal representation. */
    #define BASE 1000000000U
    /* Max number of decimal blocks needed for n up to 10000:
       5^10000 has about 6990 decimal digits, i.e. 6990/9 < 800 blocks.
       For safety we allocate a little more. */
    uint32_t digits[2000];
    int size = 1;           /* number of used blocks */
    digits[0] = 1;          /* start from 5^0 = 1 */

    for (idx_t i = 0; i < n; ++i) {
        uint64_t carry = 0;
        for (int j = 0; j < size; ++j) {
            uint64_t prod = (uint64_t)digits[j] * 5 + carry;
            digits[j] = (uint32_t)(prod % BASE);
            carry     = prod / BASE;
        }
        if (carry) {
            digits[size++] = (uint32_t)carry;
        }
    }

    /* Compute the sum of decimal digits. */
    uint64_t sum = 0;
    for (int j = 0; j < size; ++j) {
        uint32_t val = digits[j];
        /* Each block contains up to 9 decimal digits. */
        for (int d = 0; d < 9; ++d) {
            sum += val % 10;
            val /= 10;
        }
    }
    return sum;
}

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

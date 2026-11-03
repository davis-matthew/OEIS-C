/*
 * A204598  Least k such that n*k! contains every decimal digit at least once.
 * https://oeis.org/A204598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   the least k such that n*k! contains every decimal digit 0‑9 at
 *            least once
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* digits: least‑significant digit first, len = number of digits */
static bool all_digits(const uint8_t *digits, int len)
{
    unsigned seen = 0;          /* bit i set iff digit i has been seen */
    for (int i = 0; i < len; ++i)
        seen |= 1U << digits[i];
    return seen == 0x3FFU;      /* 10 bits all set */
}

/* Check whether every decimal digit appears at least once */

seq_t a(idx_t n)
{
    /* maximum digits needed for n*k! with n≤2^64−1 and k in the range
       that actually occurs (~<200 digits). Allocate a bit more for safety. */
    uint8_t digits[512];
    int len = 0;
    uint64_t t = (uint64_t)n;

    /* initialise digits from n (decimal representation) */
    if (t == 0) {
        digits[len++] = 0;
    } else {
        while (t) {
            digits[len++] = t % 10;
            t /= 10;
        }
    }

    /* k = 1 : product = n*1! = n */
    if (all_digits(digits, len))
        return (seq_t)1;

    /* iterate k = 2,3,4,... */
    for (idx_t k = 2;; ++k) {
        /* multiply current product (stored in digits) by k */
        __uint128_t carry = 0;
        for (int i = 0; i < len; ++i) {
            __uint128_t prod = (__uint128_t)digits[i] * k + carry;
            digits[i] = (uint8_t)(prod % 10);
            carry     = prod / 10;
        }
        while (carry) {
            digits[len++] = (uint8_t)(carry % 10);
            carry /= 10;
        }

        /* check if all digits appear */
        if (all_digits(digits, len))
            return (seq_t)k;
    }
}

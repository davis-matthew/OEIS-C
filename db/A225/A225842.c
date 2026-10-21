/*
 * A225842  Least k such that some digit occurs n times (or more) in the
 *          decimal representation of n^k.
 * https://oeis.org/A225842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k>0 such that some decimal digit occurs at least n
 *            * times in the decimal representation of n^k. (a(0)=0 by
 *            convention.)
 * Range:     u8 n <= 62, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_digits.h"

/* Maximum number of decimal digits needed for n up to about 10 000.
   This is ample for all k that ever need to be checked. */
#define MAX_DIGITS 200000

/* Multiply the little‑endian decimal array `digits` of length `*len`
   by the small integer `m`.  The array size is expanded as needed. */
static void mul_digits(uint8_t *digits, int *len, uint64_t m)
{
    uint64_t carry = 0;
    for (int i = 0; i < *len; ++i) {
        uint64_t prod = (uint64_t)digits[i] * m + carry;
        digits[i]   = (uint8_t)(prod % 10);
        carry      = prod / 10;
    }
    while (carry > 0) {
        digits[(*len)++] = (uint8_t)(carry % 10);
        carry            = carry / 10;
    }
}

/* The requested OEIS sequence. */
seq_t a(idx_t n)
{
    /* a(0)=0 and a(1)=0 by definition. */
    if (n <= 1) return 0;

    static uint8_t digits[MAX_DIGITS];
    int len = 0;

    /* Initialise with n^1 = n. */
    uint64_t tmp = (uint64_t)n;
    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp > 0) {
            digits[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
    }

    seq_t k = 1;
    while (1) {
        unsigned int cnt[10];
        count_digits(digits, len, cnt);
        for (int d = 0; d < 10; ++d)
            if (cnt[d] >= n) return k;
        mul_digits(digits, &len, n);
        ++k;
    }
}

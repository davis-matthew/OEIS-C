/*
 * A071789  Decimal expansion of the first (of 10) decimal selvage numbers;
 *          the n-th digit of a decimal selvage number, x, is equal to the
 *          tenths digit of n*x.
 * https://oeis.org/A071789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, base, nice
 * Formula:   a(n) = floor((n+1) * (sum_{k=0}^{n-1} a(k) * 10^{n-1-k}) /
 *            10^{n-1}) mod 10 where a(0) = 1; for n ≥ 1 the above recurrence
 *            holds
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Cached digits in reverse order: digits[0] = a_{len-1}, digits[len-1] = a0 */
static uint8_t *digits = NULL;
static size_t   cap   = 0;
static size_t   len   = 0;          /* number of digits currently stored */

seq_t a(idx_t n)
{
    /* ensure digit a0 is available */
    if (len == 0) {
        /* initialise storage for a0 */
        cap = 16;
        digits = (uint8_t *)malloc(cap);
        digits[0] = 1;   /* a0 = 1 */
        len = 1;
    }

    /* a(0) is handled directly */
    if (n == 0)
        return (seq_t)1;

    /* compute digits up to index n if not already present */
    while (len <= (size_t)n) {
        /* multiplier = index_of_new_digit + 1 */
        uint64_t multiplier = (uint64_t)(len + 1);

        uint64_t carry = 0;
        uint8_t  new_digit = 0;

        /* multiply current prefix S (length 'len') by 'multiplier'
           without altering the stored digits */
        for (size_t i = 0; i < len; ++i) {
            uint64_t prod = carry + multiplier * (uint64_t)digits[i];
            uint8_t  mod  = (uint8_t)(prod % 10);
            carry = prod / 10;
            if (i == len - 1)
                new_digit = mod;   /* the required digit */
        }

        /* ensure capacity for one more digit */
        if (len + 1 > cap) {
            size_t new_cap = cap ? cap * 2 : 16;
            while (new_cap <= len + 1)
                new_cap *= 2;
            uint8_t *tmp = (uint8_t *)realloc(digits, new_cap);
            digits = tmp;
            cap    = new_cap;
        }

        /* shift existing digits to the right to insert new_digit at front */
        memmove(&digits[1], &digits[0], len * sizeof(uint8_t));
        digits[0] = new_digit;
        ++len;
    }

    /* after loop, digits[0] holds a(n) */
    return (seq_t)digits[0];
}

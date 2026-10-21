/*
 * A217159  a(n) is the least value of k such that the decimal expansion of
 *          n^k contains four consecutive identical digits.
 * https://oeis.org/A217159
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_four_consecutive
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A217159 – Least exponent k such that the decimal expansion of n^k
 * contains four consecutive identical digits.
 *
 * This implementation multiplies repeatedly by n using an explicit
 * decimal big‑integer representation in an array of base‑10 digits.
 * It scans the digit array after each multiplication for four equal
 * consecutive digits.  Because the exponent k for n ≤ 10 000 is
 * modest (≤ few hundred), the array size is fixed at 20 000 digits,
 * which is more than enough for the required range.
 *
 * The function has external linkage and conforms to the required
 * type signature.  No external utilities are used; the algorithm
 * works for any numerically adequate seq_t and idx_t.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/has_four_consecutive.h"

#undef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))

seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* undefined for n < 2, but safe */

    /* Largest possible number of decimal digits for n^k when k <= 2000
     * and n <= 10 000 (4 decimal digits).  2000*4 = 8000 < 20 000. */
    enum { MAX_DIGITS = 20000 };
    uint8_t digits[MAX_DIGITS];
    size_t len = 0;

    /* initialize digits with n^1 */
    uint64_t temp = (uint64_t)n;
    while (temp) {
        digits[len++] = (uint8_t)(temp % 10);
        temp /= 10;
    }
    if (len == 0) digits[len++] = 0;    /* n = 0 case, not used here */

    /* check for four consecutive equal digits in n^1 */
    if (has_four_consecutive(digits, len))
        return (seq_t)1;

    /* iterate k = 2,3,... */
    for (seq_t k = 2; ; ++k) {
        /* multiply current value by n, using 64‑bit intermediates */
        uint64_t carry = 0;
        for (size_t i = 0; i < len; ++i) {
            uint64_t prod = (uint64_t)digits[i] * (uint64_t)n + carry;
            digits[i] = (uint8_t)(prod % 10);
            carry = prod / 10;
        }
        while (carry) {
            if (len >= MAX_DIGITS) {
                /* Should never happen for the required range.
                 * In a pathological setting we stop conservatively. */
                return (seq_t)0;
            }
            digits[len++] = (uint8_t)(carry % 10);
            carry /= 10;
        }

        if (has_four_consecutive(digits, len))
            return k;
    }
}

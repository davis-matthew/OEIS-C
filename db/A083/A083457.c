/*
 * A083457  Smallest nontrivial k such that k^n - 1 is a palindrome (k >1 for
 *          n>1).
 * https://oeis.org/A083457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search method: for n>1 test k=2…9 in order.
   For each k compute k^n – 1 in base 10, check if it is a palindrome.
   If none of k=2…9 works, return 10 (10^n – 1 is always 999…9).
   a(1)=1. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define MAX_DIGITS 12000   /* Enough for 9^10000 (≈9546 digits) */

static uint8_t digits[MAX_DIGITS];  /* reused buffer for each k */

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }

    /* k = 2 … 9 */
    for (idx_t k = 2; k < 10; ++k) {
        /* start with value 1 */
        int len = 1;
        digits[0] = 1;

        /* compute k^n by repeated multiplication */
        for (idx_t e = 0; e < n; ++e) {
            int carry = 0;
            for (int i = 0; i < len; ++i) {
                int prod = digits[i] * (int)k + carry;
                digits[i] = (uint8_t)(prod % 10);
                carry     = prod / 10;
            }
            while (carry) {
                digits[len++] = (uint8_t)(carry % 10);
                carry /= 10;
            }
        }

        /* subtract 1 (k^n – 1) */
        int j = 0;
        while (j < len && digits[j] == 0) {
            digits[j] = 9;
            ++j;
        }
        if (j < len) {
            digits[j] -= 1;
        }

        /* trim possible leading zero after subtraction */
        if (len > 1 && digits[len - 1] == 0) {
            --len;
        }

        /* check palindrome */
        bool ok = true;
        int left = 0, right = len - 1;
        while (left < right) {
            if (digits[left] != digits[right]) {
                ok = false;
                break;
            }
            ++left;
            --right;
        }

        if (ok) {
            return (seq_t)k;
        }
    }

    /* 10 always works: 10^n – 1 = 999…9 */
    return (seq_t)10;
}

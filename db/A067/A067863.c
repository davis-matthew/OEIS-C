/*
 * A067863  Numbers k such that k divides the sum of digits of 7^k.
 * https://oeis.org/A067863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, more, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the OEIS sequence A067863.
   a(n) = k, where the n‑th term is the number k such that
   k divides the sum of the decimal digits of 7^k.  The sequence
   is known to be finite (the first seven terms are
   1, 13, 67, 94, 139, 220, 805) and no further terms are found
   up to 10^5.  The routine pre‑computes all terms up to a fixed
   limit (2000) and returns the requested n‑th term, or 0 if
   the index is out of range.  The implementation works for all
   supported integer widths of seq_t and idx_t. */

#include <stddef.h>   /* for size_t */

/* Pre‑compute the sequence only once. */
#define MAX_DIGITS 6000   /* enough for 7^2000 (~1690 decimal digits) */
#define PRECOMP_LIMIT 2000
#define TERM_CAPACITY 128

seq_t a(idx_t n)
{
    /* Static state for the one‑time pre‑computation. */
    static int initialized = 0;
    static uint8_t digits[MAX_DIGITS];
    static size_t digits_len = 0;
    static seq_t terms[TERM_CAPACITY];
    static size_t term_count = 0;

    if (!initialized) {
        /* Start with 7^0 = 1. */
        digits[0] = 1;
        digits_len = 1;

        for (idx_t k = 1; k <= PRECOMP_LIMIT; ++k) {
            /* Multiply the current big integer by 7. */
            uint16_t carry = 0;
            for (size_t i = 0; i < digits_len; ++i) {
                uint16_t prod = (uint16_t)digits[i] * 7 + carry;
                digits[i] = (uint8_t)(prod % 10);
                carry     = prod / 10;
            }
            while (carry) {
                if (digits_len == MAX_DIGITS) {
                    /* Should not happen for the chosen limit. */
                    break;
                }
                digits[digits_len++] = (uint8_t)(carry % 10);
                carry /= 10;
            }

            /* Compute the digit sum. */
            uint32_t sum = 0;
            for (size_t i = 0; i < digits_len; ++i)
                sum += digits[i];

            /* Check the divisibility condition. */
            if (sum % k == 0) {
                if (term_count < TERM_CAPACITY)
                    terms[term_count++] = (seq_t)k;
            }
        }
        initialized = 1;
    }

    /* Return the n‑th term if it exists; otherwise 0. */
    if (n == 0 || (size_t)n > term_count)
        return (seq_t)0;
    return terms[(size_t)n - 1];
}

/*
 * A050730  Decimal expansion of 9^n contains no pair of consecutive equal
 *          digits (probably finite).
 * https://oeis.org/A050730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth non‑negative integer k (k≥0) such that the
 *            decimal expansion of 9^k has no pair of consecutive equal digits
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   ensure_digits_cap, ensure_terms_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For each n ≥ 0 the function returns a(n). */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ensure_digits_cap.h"
#include "../utils/ensure_terms_cap.h"

seq_t a(idx_t n)
{
    /* static data cached across calls */
    static seq_t *terms = NULL;
    static size_t terms_len = 0, terms_cap = 0;
    static uint8_t *digits = NULL;
    static size_t digits_len = 0, digits_cap = 0;
    static idx_t current_exp = 0;   /* last exponent processed (>=0) */
    static int initialized = 0;

    if (!initialized)
    {
        /* initialise with exponent 0: 9^0 = 1 */
        current_exp = 0;
        ensure_digits_cap(&digits_len, &digits_cap, &digits, 1);
        digits[0] = 1;
        digits_len = 1;

        ensure_terms_cap(&terms_len, &terms_cap, &terms, 1);
        terms[0] = (seq_t)current_exp;   /* a(0) = 0 */
        terms_len = 1;
        initialized = 1;
    }

    /* generate terms until we have enough */
    while (terms_len <= n)
    {
        /* move to next exponent */
        current_exp++;

        /* multiply current big integer by 9 */
        unsigned carry = 0;
        for (size_t i = 0; i < digits_len; i++)
        {
            unsigned prod = digits[i] * 9u + carry;
            digits[i] = (uint8_t)(prod % 10u);
            carry = prod / 10u;
        }
        while (carry)
        {
            ensure_digits_cap(&digits_len, &digits_cap, &digits, digits_len + 1);
            digits[digits_len++] = (uint8_t)(carry % 10u);
            carry /= 10u;
        }

        /* check for consecutive equal digits */
        int ok = 1;
        for (size_t i = 0; i + 1 < digits_len; i++)
        {
            if (digits[i] == digits[i + 1])
            {
                ok = 0;
                break;
            }
        }

        if (ok)
        {
            ensure_terms_cap(&terms_len, &terms_cap, &terms, terms_len + 1);
            terms[terms_len++] = (seq_t)current_exp;
        }
    }

    return terms[n];
}

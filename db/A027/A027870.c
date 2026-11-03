/*
 * A027870  Number of zero digits in 2^n.
 * https://oeis.org/A027870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the number of zero digits in the decimal representation
 *            of 2^n
 * Range:     u8 n <= 7359, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9912 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: construct 2^n in base‑10 using an array of digits, multiply by 2 n times,
   and count the occurrences of 0. This works for any seq_t, including 64‑bit or arbitrary‑precision types. */

#include <stdlib.h>

static seq_t count_zero_digits_of_2_pow(idx_t n)
{
    /* Special case */
    if (n == 0) return (seq_t)0;          /* 2^0 = 1 has no zero digits */

    /* Rough upper bound on decimal digits of 2^n: floor(n*log10(2)) + 1 */
    /* log10(2) ≈ 0.30103, add a few extra bytes for safety */
    size_t max_digits = (size_t)((double)n * 0.30103) + 10;
    unsigned char *digits = (unsigned char*)malloc(max_digits);
    if (!digits) return (seq_t)0;         /* malloc failure => return 0 (unlikely) */

    digits[0] = 1;   /* 2^0 = 1 */
    size_t len = 1;

    for (idx_t i = 0; i < n; ++i) {
        unsigned int carry = 0;
        for (size_t j = 0; j < len; ++j) {
            unsigned int prod = digits[j] * 2 + carry;
            digits[j] = prod % 10;
            carry = prod / 10;
        }
        while (carry) {
            digits[len++] = carry % 10;
            carry /= 10;
        }
    }

    /* Count zeros */
    seq_t zero_cnt = (seq_t)0;
    for (size_t i = 0; i < len; ++i) {
        if (digits[i] == 0) ++zero_cnt;
    }

    free(digits);
    return zero_cnt;
}

seq_t a(idx_t n)
{
    return count_zero_digits_of_2_pow(n);
}

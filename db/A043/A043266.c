/*
 * A043266  Sum of the digits of the n-th base 7 palindrome.
 * https://oeis.org/A043266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of digits of the n-th base 7 palindrome
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   pow7_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the sequence starts at n = 1 with value a(1) = 0. */

#include <stdint.h> /* for UINT64_MAX */
#include "../utils/pow7_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety, although n ≥ 1 by definition */

    idx_t remaining = n;           /* how many palindromes we still need to skip */
    idx_t length     = 1;          /* current palindrome length being considered */

    while (1) {
        idx_t count;               /* how many palindromes of this length exist */

        if (length == 1) {
            count = 7;             /* 0 … 6 */
        } else {
            uint32_t half = (uint32_t)((length + 1) / 2);   /* ceil(L/2) */
            uint64_t p = pow7_u64(half - 1);                /* 7^(half-1) */
            if (p > UINT64_MAX / 6ULL) count = UINT64_MAX;
            else                     count = 6ULL * p;     /* 6 * 7^(half-1) */
        }

        if (remaining > count) {
            remaining -= count;
            ++length;
            continue;
        }

        /* Remaining palindromes are within current length */
        if (length == 1) {
            /* digits 0 … 6 */
            return (seq_t)(remaining - 1);
        }

        /* L > 1 */
        uint32_t half = (uint32_t)((length + 1) / 2);
        uint64_t p = pow7_u64(half - 1);
        uint64_t idx0 = remaining - 1;   /* zero‑based index inside this length */

        uint64_t lead_inc = idx0 / p;
        uint8_t lead_digit = (uint8_t)(lead_inc + 1); /* 1 … 6 */

        uint64_t rem = idx0 % p;
        seq_t sum_prefix = lead_digit;
        uint8_t middle_digit = 0;

        for (uint32_t pos = 0; pos < half - 1; ++pos) {
            uint8_t digit = (uint8_t)(rem % 7);
            sum_prefix += digit;
            if (length & 1 && pos == half - 2)
                middle_digit = digit;   /* middle digit for odd length */
            rem /= 7;
        }

        if ((length & 1) == 0) { /* even length: mirror all digits */
            return sum_prefix * 2;
        } else {                /* odd length: middle digit counted once */
            return sum_prefix * 2 - middle_digit;
        }
    }
}

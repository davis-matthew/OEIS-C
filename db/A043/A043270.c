/*
 * A043270  Sum of the digits of the n-th base 11 palindrome.
 * https://oeis.org/A043270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of the base‑11 digits of the n‑th base‑11 palindrome
 *            (offset 1)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   pow11u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow11u.h"

seq_t a(idx_t n)
{
    /* The first 11 palindromes are the single‑digit numbers 0…10. */
    if (n <= (idx_t)11)
        return (seq_t)(n - 1);

    /* Remaining palindromes start at length 2. */
    idx_t rem = n - 11;   /* 1‑based offset into the list of palindromes with len≥2 */
    idx_t len = 2;        /* current palindrome length */

    /* Find the length that contains the nth palindrome. */
    for (;; ++len) {
        idx_t m = (len + 1) / 2;            /* number of independent digits */
        idx_t pow_val = pow11u(m - 1);      /* 11^(m-1) */
        idx_t count = 10 * pow_val;         /* palindromes of this length */

        if (rem <= count)
            break;
        rem -= count;
    }

    /* 0‑based index within palindromes of this length. */
    idx_t k = rem - 1;

    /* Decode the independent digits. */
    idx_t m = (len + 1) / 2;
    idx_t pow_val = pow11u(m - 1);
    idx_t first = k / pow_val + 1;          /* leading digit (1–10) */
    idx_t rest  = k % pow_val;

    /* Store the m independent digits. */
    unsigned digits[12] = {0};              /* 12 is enough for n up to 10⁴ */
    digits[0] = (unsigned)first;

    for (idx_t i = 1; i < m; ++i) {
        pow_val /= (idx_t)11;
        digits[i] = (unsigned)(rest / pow_val);
        rest      = rest % pow_val;
    }

    /* Sum the first half of the palindrome. */
    idx_t sum = 0;
    for (idx_t i = 0; i < m; ++i)
        sum += (idx_t)digits[i];

    /* Extend to the full palindrome and return the digit sum. */
    seq_t result;
    if (len % 2 == 0) {
        result = (seq_t)(sum * 2);
    } else {
        result = (seq_t)(sum * 2 - (idx_t)digits[m - 1]);
    }
    return result;
}

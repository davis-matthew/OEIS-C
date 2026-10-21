/*
 * A065718  Number of 7's in decimal expansion of 2^n.
 * https://oeis.org/A065718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of digit '7' in the decimal expansion of 2^n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute 2^n as a decimal number stored in an array of base‑10 digits,
   then count how many digits equal 7.  The algorithm is O(n · digits),
   which is easily fast enough for n up to about 10000. */
seq_t a(idx_t n) {
    /* Rough upper bound for the number of decimal digits of 2^n:
       digits ≈ n * log10(2) + 1,  log10(2) ≈ 0.30103.
       Allocate a little extra space to avoid reallocation. */
    size_t max_digits = (size_t)(n * 4 + 10);
    uint8_t *digits = (uint8_t *)malloc(max_digits);
    if (!digits) return 0;          /* Allocation failed – return 0 as a fallback. */

    digits[0] = 1;                  /* start with value 1 */
    size_t len = 1;                  /* current number of digits, least‑significant first */

    for (idx_t i = 0; i < n; ++i) {
        uint32_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint32_t val = digits[j] * 2U + carry;
            digits[j] = (uint8_t)(val % 10U);
            carry = val / 10U;
        }
        while (carry) {
            if (len >= max_digits) {   /* safety check – should not happen for n≤10000 */
                free(digits);
                return 0;
            }
            digits[len++] = (uint8_t)(carry % 10U);
            carry /= 10U;
        }
    }

    /* Count occurrences of digit '7' */
    seq_t cnt = 0;
    for (size_t j = 0; j < len; ++j) {
        if (digits[j] == 7U) cnt += 1;
    }

    free(digits);
    return cnt;
}

/*
 * A085769  Total number of zeros in the decimal expansions of 2^n and 5^n.
 * https://oeis.org/A085769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the total number of zeros appearing in the decimal
   representations of 2ⁿ and 5ⁿ.
   The function computes the powers directly by repeated multiplication
   in base 10, using only integer arithmetic and dynamic arrays. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Count the zeros in baseᵏⁿ using decimal arithmetic.
   base is 2 or 5. */
static seq_t zeros_of_power(unsigned char base, idx_t n)
{
    /* Allocate an array to hold decimal digits (least‑significant first). */
    size_t len = 1;      /* number of used digits */
    size_t cap = 1;      /* allocated capacity */
    unsigned char *digits = (unsigned char *)malloc(cap);
    if (!digits) return 0;            /* fallback if allocation fails */
    digits[0] = 1;                    /* start from 1 */

    /* Repeatedly multiply by the base. */
    for (idx_t i = 0; i < n; ++i) {
        unsigned carry = 0;
        for (size_t j = 0; j < len; ++j) {
            unsigned prod = digits[j] * base + carry;
            digits[j] = prod % 10;
            carry = prod / 10;
        }
        /* Grow the array if necessary. */
        while (carry) {
            if (len == cap) {
                cap <<= 1;                          /* double capacity */
                unsigned char *tmp = (unsigned char *)realloc(digits, cap);
                if (!tmp) { free(digits); return 0; }
                digits = tmp;
            }
            digits[len++] = carry % 10;
            carry /= 10;
        }
    }

    /* Count zeros among the digits. */
    seq_t zeros = 0;
    for (size_t j = 0; j < len; ++j) {
        if (digits[j] == 0) {
            ++zeros;
        }
    }
    free(digits);
    return zeros;
}

seq_t a(idx_t n)
{
    seq_t z2 = zeros_of_power(2, n);
    seq_t z5 = zeros_of_power(5, n);
    return z2 + z5;
}

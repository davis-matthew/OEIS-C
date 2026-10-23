/*
 * A169685  Decimal expansion of 2^521 - 1.
 * https://oeis.org/A169685
 *
 * Offset:    157    (a(157) is the first term)
 * Keywords:  nonn, cons, easy, fini, full
 * Formula:   a(n) = decimal digit of (2^521 ‑ 1) at position n, where a(157)
 *            is the most significant digit
 * Range:     u8 n <= 313, u16 n <= 313, u32 n <= 313, u64 n <= 313,
 *            u128 n <= 313, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Precomputed decimal digits of 2^521‑1 (157 digits). */
static seq_t digits[200];
static bool ready = false;
static const idx_t OFFSET = 157;      /* a(157) is the first digit */
static const idx_t NUM_DIGITS = 157;  /* total number of digits */

/* Compute the decimal digits of 2^521 ‑ 1 and store in digits[]. */
static void compute_digits(void)
{
    uint8_t dec[200] = {0};
    size_t len = 1;
    dec[0] = 1;                    /* start with 1 */

    /* Multiply by 2, 521 times. */
    for (idx_t k = 0; k < 521; ++k) {
        uint8_t carry = 0;
        for (size_t i = 0; i < len; ++i) {
            uint8_t prod = dec[i] * 2 + carry;
            dec[i] = prod % 10;
            carry  = prod / 10;
        }
        while (carry) {
            dec[len++] = carry % 10;
            carry /= 10;
        }
    }

    /* Subtract 1 from the big integer represented in dec[] (little‑endian). */
    size_t pos = 0;
    while (pos < len && dec[pos] == 0) {
        dec[pos] = 9;
        ++pos;
    }
    if (pos < len) {
        dec[pos] -= 1;
    }

    /* Store digits in most‑significant‑first order in digits[]. */
    for (size_t i = 0; i < len; ++i) {
        digits[i] = (seq_t)dec[len - 1 - i];
    }

    ready = true;
}

/* Return the n-th digit of 2^521 ‑ 1. */
seq_t a(idx_t n)
{
    if (n < OFFSET) {
        /* Undefined for indices below the offset; return 0. */
        return (seq_t)0;
    }

    if (!ready) {
        compute_digits();
    }

    idx_t pos = n - OFFSET;
    if (pos >= NUM_DIGITS) {
        /* Undefined for indices beyond the decimal expansion; return 0. */
        return (seq_t)0;
    }

    return digits[pos];
}

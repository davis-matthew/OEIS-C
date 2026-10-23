/*
 * A021293  Decimal expansion of 1/289.
 * https://oeis.org/A021293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth digit of the decimal expansion of 1/289 (periodic
 *            with length 272)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// It is convenient to pre‑compute the 272‑digit repetend once
#include <stdint.h>
#include <stdbool.h>

static bool _digits_ready = false;
static unsigned char _digits[272];

/* Precompute the digits of 1/289.  The ordinary long division
   algorithm is used; the intermediate value (rem*10) never
   exceeds 2890, so a 32‑bit unsigned integer is sufficient. */
static void _compute_digits(void)
{
    uint32_t rem = 1;          // remainder after the decimal point
    for (int i = 0; i < 272; ++i) {
        uint32_t tmp = rem * 10U;
        _digits[i] = (unsigned char)(tmp / 289U);  // current digit
        rem       = tmp % 289U;                  // new remainder
    }
    _digits_ready = true;
}

seq_t a(idx_t n)
{
    if (! _digits_ready)
        _compute_digits();

    /* The decimal expansion repeats every 272 digits. */
    idx_t pos = n % 272U;
    return (seq_t)_digits[pos];
}

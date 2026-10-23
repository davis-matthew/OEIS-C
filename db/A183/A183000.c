/*
 * A183000  Decimal expansion of the integer c^3 where c = 299792458 is the
 *          exact speed of light in vacuum (m/s).
 * https://oeis.org/A183000
 *
 * Offset:    26    (a(26) is the first term)
 * Keywords:  cons, nonn, fini, full
 * Formula:   a(n) = digit of 299792458^3, with a(26) the most significant
 *            digit
 * Range:     u8 n <= 51, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/*
 * Compute and cache the 26 decimal digits of 299792458^3.
 * The digits are stored with digits[0] = most significant,
 * digits[25] = least significant.
 */
static uint8_t digits[26];
static bool ready = false;

static void init_digits(void)
{
    /* 128‑bit little‑endian word array (w[0] = least significant 32 bits) */
    uint32_t w[4] = { 299792458u, 0, 0, 0 };

    /* Multiply twice to obtain the cube. */
    for (int iter = 0; iter < 2; ++iter) {
        uint64_t carry = 0;
        for (int i = 0; i < 4; ++i) {
            uint64_t prod = (uint64_t)w[i] * 299792458u + carry;
            w[i] = (uint32_t)prod;
            carry = prod >> 32;
        }
        /* carry will be zero because the value fits in 128 bits. */
    }

    /* Make a copy for successive divisions by 10. */
    uint32_t temp[4];
    for (int i = 0; i < 4; ++i) temp[i] = w[i];

    /* Extract digits from least significant to most significant. */
    for (int pos = 25; pos >= 0; --pos) {
        uint64_t rem = 0;
        for (int i = 3; i >= 0; --i) {
            uint64_t cur = (rem << 32) | temp[i];
            temp[i] = (uint32_t)(cur / 10);
            rem     = cur % 10;
        }
        digits[pos] = (uint8_t)rem;
    }
}

seq_t a(idx_t n)
{
    /* Supply the offset 26 as per OEIS definition. */
    const idx_t OFFSET = 26;

    if (n < OFFSET)
        return (seq_t)0;          /* undefined part of the sequence */

    /* Lazy initialization of the digit array. */
    if (!ready) {
        init_digits();
        ready = true;
    }

    idx_t pos = n - OFFSET;      /* zero‑based index into digits[] */

    if (pos < 26)
        return (seq_t)digits[pos];
    else
        return (seq_t)0;          /* the sequence is defined as 0 beyond the digits */
}

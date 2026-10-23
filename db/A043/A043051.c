/*
 * A043051  a(n)=(s(n)+3)/5, where s(n)=n-th base 5 palindrome that starts
 *          with 2.
 * https://oeis.org/A043051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+3)/5, where s(n) is the n‑th base‑5 palindrome
 *            starting with digit 2. The function finds that palindrome and
 *            returns (value+3)/5
 * Range:     u8 n <= 13, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define MAXL 64

seq_t a(idx_t n)
{
    idx_t remaining = n;
    unsigned L;

    /* Find the length L of the desired palindrome. */
    for (L = 1; ; ++L)
    {
        unsigned k = L / 2;
        unsigned exponent = (L % 2 == 0) ? (k - 1) : k;

        idx_t count = 1;
        for (unsigned i = 0; i < exponent; ++i) count *= 5;

        if (remaining <= count)
            break;
        remaining -= count;
    }

    /* Index inside the block (0‑based). */
    idx_t r = remaining - 1;
    unsigned k      = L / 2;
    unsigned exponent = (L % 2 == 0) ? (k - 1) : k;

    /* Extract the freed digits of r in base‑5 (least significant first). */
    idx_t base5[MAXL] = {0};
    for (unsigned i = 0; i < exponent; ++i)
    {
        base5[i] = r % 5;
        r /= 5;
    }

    /* Build the full base‑5 palindrome. */
    uint8_t digits[MAXL];

    digits[0] = 2;               /* leading digit */
    unsigned pos = 1;

    /* Left side free digits (most significant first). */
    for (int i = (int)exponent; i > 0; --i)
        digits[pos++] = (uint8_t)base5[i - 1];

    /* Mirror to the right side. */
    for (unsigned j = 0; j < k; ++j)
        digits[L - 1 - j] = digits[j];

    /* Convert the base‑5 digits to decimal. */
    idx_t val = 0;
    for (unsigned i = 0; i < L; ++i)
        val = val * 5 + digits[i];

    return (seq_t)((val + 3) / 5);
}

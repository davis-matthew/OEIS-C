/*
 * A024651  n written in fractional base 9/3.
 * https://oeis.org/A024651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 647, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Compute a(n) for OEIS A024651: representation of n in
 * fractional base 9/3 (digits 0‑8).  For each digit d we have
 * 0 ≤ d < 9; if a digit would exceed 8 we subtract 9 and carry 3 to the
 * next higher position.  The resulting digits are interpreted as decimal
 * digits of a(n). */
 
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* store digits least‑significant first */
    int digits[128];
    int cnt = 0;
    const idx_t a_base = 9;
    const idx_t b_base = 3;
    idx_t current = n;

    while (current > 0) {
        idx_t digit = current % a_base;          /* 0..8 */
        digits[cnt++] = (int)digit;
        current = ((current - digit) / a_base) * b_base;
    }

    seq_t value = 0;
    /* build decimal value from most‑significant digit */
    for (int i = cnt - 1; i >= 0; --i) {
        value = value * 10 + (seq_t)digits[i];
    }
    return value;
}

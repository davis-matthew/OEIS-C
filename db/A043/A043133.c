/*
 * A043133  Numbers k such that 0 and 3 occur juxtaposed in the base-7
 *          representation of k but not of k-1.
 * https://oeis.org/A043133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A043133: numbers k whose base‑7 representation
 *  contains the adjacent digits 0 and 3 (either order) while k‑1 does not.
 *  The function returns the nth such integer (offset 1). */
#include <stdbool.h>

static bool has_adjacent_03(seq_t x)
{
    if (x == 0) return false;
    /* Extract base‑7 digits into a small buffer.  The number of digits
     * of any seq_t value is <200 for every supported size. */
    const unsigned int MAX_DIGITS = 200;
    uint8_t digits[MAX_DIGITS];
    unsigned int len = 0;
    seq_t t = x;
    while (t > 0) {
        digits[len++] = (uint8_t)(t % (seq_t)7);
        t /= (seq_t)7;
    }
    /* Check every consecutive pair for 0,3 or 3,0. */
    for (unsigned int i = 1; i < len; ++i) {
        if ((digits[i] == 0 && digits[i-1] == 3) ||
            (digits[i] == 3 && digits[i-1] == 0))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for the OEIS offset 1 */
    idx_t found = 0;
    seq_t k = 0;
    while (found < n) {
        ++k;
        if (has_adjacent_03(k) && !has_adjacent_03(k - 1))
            ++found;
    }
    return k;
}

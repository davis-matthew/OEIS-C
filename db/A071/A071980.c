/*
 * A071980  If n = abcd (say) in decimal, then a(n) = a + ab + abc + abcd +
 *          bcd + cd + d.
 * https://oeis.org/A071980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = a + ab + abc + abcd + bcd + cd + d (sum of all prefixes
 *            and all suffixes of the decimal representation of n, excluding n
 *            itself), computed from the decimal digits of n
 * Range:     u8 n <= 166, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle zero specially. */
    if (n == 0) return (seq_t)0;

    uint8_t dig[20];          /* digits in reverse order, least significant first */
    size_t len = 0;
    idx_t tmp = n;

    /* Extract digits. */
    while (tmp > 0) {
        dig[len++] = tmp % 10;
        tmp /= 10;
    }

    /* Sum prefixes (most significant to least). */
    seq_t sum = 0;
    seq_t prefix = 0;
    for (int i = (int)len - 1; i >= 0; --i) {
        prefix = prefix * 10 + (seq_t)dig[i];
        sum += prefix;
    }

    /* Sum suffixes (most significant excluded). */
    seq_t suffix = 0;
    seq_t mult = 1;
    for (size_t i = 0; i < len; ++i) {
        suffix = (seq_t)dig[i] * mult + suffix;
        if (i + 1 < len)  /* skip the whole number itself */
            sum += suffix;
        mult *= 10;
    }

    return sum;
}

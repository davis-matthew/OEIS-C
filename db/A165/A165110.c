/*
 * A165110  a(n) = image of n under the base-9 Kaprekar map n -> (n with
 *          digits sorted into descending order) - (n with digits sorted into
 *          ascending order).
 * https://oeis.org/A165110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (digits of n in base‑9 sorted descending) - (digits of n
 *            in base‑9 sorted ascending)
 * Range:     u8 n <= 84, u16 n <= 9492, u32 n <= 9492, u64 n <= 9492,
 *            u128 n <= 9492, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle n = 0 explicitly (although the next code would also work) */
    if (n == 0)
        return (seq_t)0;

    /* Count the occurrences of each base‑9 digit (0–8). */
    unsigned char freq[9] = {0};
    idx_t tmp = n;
    while (tmp > 0) {
        unsigned int d = (unsigned int)(tmp % 9);
        freq[d]++;
        tmp /= 9;
    }

    /* Build the ascending and descending numbers in base‑9. */
    seq_t asc = 0;
    seq_t desc = 0;

    for (int d = 0; d <= 8; ++d) {
        for (int k = 0; k < freq[d]; ++k)
            asc = asc * (seq_t)9 + (seq_t)d;
    }

    for (int d = 8; d >= 0; --d) {
        for (int k = 0; k < freq[d]; ++k)
            desc = desc * (seq_t)9 + (seq_t)d;
    }

    return desc - asc;
}

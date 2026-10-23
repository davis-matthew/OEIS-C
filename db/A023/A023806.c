/*
 * A023806  Xenodromes: all digits in base 12 are different.
 * https://oeis.org/A023806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, easy
 * Formula:   not stated
 * Range:     u8 n <= 216, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   perm_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm for OEIS sequence A023806: returns the n‑th xenodrome (base‑12 number with all distinct digits). */
#include <stdint.h>
#include "../utils/perm_idx.h"

seq_t a(idx_t n)
{
    if (n <= 1u)
        return 0;                     /* a(1) = 0 */

    /* Convert to 0‑based index among positive distinct‑digit numbers. */
    idx_t idx = n - 1;                /* exclude the leading zero */

    /* Find the number of digits of the result. */
    idx_t length = 1;
    idx_t cumulative = 0;
    idx_t count_len = 0;
    while (1) {
        if (length == 1)
            count_len = 11;                         /* digits 1…11 */
        else
            count_len = 11 * perm_idx(11, length - 1); /* leading digit 1‑11, then permutations */
        if (idx <= cumulative + count_len)
            break;
        cumulative += count_len;
        ++length;
    }

    idx_t offset = idx - cumulative - 1;  /* 0‑based offset within this length */

    /* Generate the digits of the number in base 12. */
    uint8_t digits[12];
    uint8_t used[12] = {0};
    uint8_t used_cnt = 0;

    for (uint8_t pos = 0; pos < length; ++pos) {
        uint8_t start = (pos == 0) ? 1 : 0;   /* first digit cannot be 0 */
        for (uint8_t d = start; d < 12; ++d) {
            if (used[d])
                continue;

            uint8_t remaining_positions = length - pos - 1;
            uint8_t remaining_digits   = 12 - used_cnt - 1;
            idx_t   cnt = perm_idx(remaining_digits, remaining_positions);

            if (offset < cnt) {
                digits[pos] = d;
                used[d] = 1;
                ++used_cnt;
                break;
            } else {
                offset -= cnt;
            }
        }
    }

    /* Convert the digit array to an integer value in decimal. */
    seq_t value = 0;
    for (uint8_t i = 0; i < length; ++i)
        value = value * 12u + (seq_t)digits[i];

    return value;
}

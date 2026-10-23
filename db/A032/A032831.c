/*
 * A032831  Numbers whose set of base-7 digits is {3,4}.
 * https://oeis.org/A032831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 94, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-7 digits are only 3 or 4 (sequence A032831). */
#include <stdint.h>

seq_t a(idx_t n) {
    /* Find the length of the base‑7 representation. */
    uint64_t len = 1;
    uint64_t cumulative = 2;           /* terms with lengths <= 1 */
    while (n > cumulative) {
        cumulative = cumulative * 2 + 2;   /* cumulative for next length */
        len++;
    }

    /* Position of the number inside the group of length `len`. */
    uint64_t pos = n - (cumulative - ((uint64_t)1 << len));
    uint64_t index = pos - 1;           /* 0‑based index for this length */

    /* Build the number from most significant digit to least. */
    seq_t result = 0;
    for (uint64_t i = 0; i < len; ++i) {
        uint64_t bit = (index >> (len - 1 - i)) & 1;
        result = result * (seq_t)7 + (bit ? (seq_t)4 : (seq_t)3);
    }
    return result;
}

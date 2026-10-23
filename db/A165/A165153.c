/*
 * A165153  a(n) = the product of all distinct positive (nonzero) integers
 *          that, when written in binary, occur as substrings in the binary
 *          representation of n.
 * https://oeis.org/A165153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = product of all distinct positive integers that occur as
 *            binary substrings of the binary representation of n
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 54, u64 n <= 183,
 *            u128 n <= 751, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Determine the binary length of n. */
    idx_t len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        ++len;
        tmp >>= 1;
    }

    /* Buffer to keep distinct substrings (max 2080 for 64‑bit n). */
    static const size_t MAX_SUBSTR = 4000;
    uint64_t seen[MAX_SUBSTR];
    size_t seen_cnt = 0;

    seq_t product = 1;

    for (idx_t i = 0; i < len; ++i) {
        seq_t val = 0;
        for (idx_t j = i; j < len; ++j) {
            /* Extract bit at position j (0 = MSB). */
            seq_t bit = (n >> (len - 1 - j)) & 1;
            val = (val << 1) | bit;
            if (val == 0) continue;  /* ignore non‑positive substrings */

            /* Check if this value already seen. */
            bool already = false;
            uint64_t uval = (uint64_t)val;  /* val fits in 64 bits */
            for (size_t k = 0; k < seen_cnt; ++k) {
                if (seen[k] == uval) { already = true; break; }
            }
            if (!already) {
                seen[seen_cnt++] = uval;
                product *= val;
            }
        }
    }
    return product;
}

/*
 * A023757  Plaindromes: numbers whose digits in base 16 are in nondecreasing
 *          order.
 * https://oeis.org/A023757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th non-negative integer whose hexadecimal digits
 *            are in nondecreasing order (from most significant to least)
 * Range:     u8 n <= 136, u16 n <= 3876, u32 n <= 6000, u64 n <= 6000,
 *            u128 n <= 6000, big any n
 * Validated: standard, pass, 86 terms through n = 5535 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool is_nondecreasing_hex(seq_t val)
{
    unsigned last = 16u;          /* larger than any hex digit */
    while (val) {
        unsigned digit = (unsigned)(val & 0xFULL); /* extract lowest hex digit */
        if (digit > last) return false;          /* decreasing pair found */
        last = digit;
        val >>= 4;                               /* move to next digit */
    }
    return true;                               /* zero and all checked numbers */
}

seq_t a(idx_t n)
{
    seq_t count = 0;       /* number of valid integers found so far */
    seq_t x = 0;           /* candidate integer */

    for (;;) {
        if (is_nondecreasing_hex(x)) {
            count++;
            if (count == n) return x;
        }
        ++x;
    }
}

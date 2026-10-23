/*
 * A044084  Numbers n such that string 1,2 occurs in the base 5 representation
 *          of n but not of n-1.
 * https://oeis.org/A044084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A044084: numbers n such that the base‑5 representation of n
 * contains the substring "12" but the base‑5 representation of n‑1 does
 * not. The function computes the n‑th such number by a linear scan
 * (an O(n log n) algorithm with a very small constant factor).
 * Offsets: a(1) = 7, a(2) = 32, …
 */

#include <stdint.h>

/* returns 1 if x (in base 5) contains the substring "12", 0 otherwise */
static inline int has12(seq_t x)
{
    /* 0 has no digits that form "12" */
    if (x == 0)
        return 0;

    /* store base‑5 digits (least significant first) */
    uint8_t digits[256];
    int len = 0;

    while (x != 0) {
        digits[len++] = (uint8_t)(x % 5);
        x /= 5;
    }

    /* scan from most significant to least significant */
    for (int i = len - 1; i > 0; --i) {
        if (digits[i] == 1 && digits[i - 1] == 2)
            return 1;
    }
    return 0;
}

seq_t a(idx_t n)
{
    seq_t candidate = 1;
    idx_t count = 0;

    for (;;) {
        if (has12(candidate) && !has12(candidate - 1)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }
}

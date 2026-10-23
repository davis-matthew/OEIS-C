/*
 * A044644  Numbers k such that string 1,3 occurs in the base 9 representation
 *          of k but not of k+1.
 * https://oeis.org/A044644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   compute by brute‑force search: k such that the base‑9
 *            representation of k contains the substring "13" but the base‑9
 *            representation of k+1 does not
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool has13(seq_t x)
{
    uint8_t prev = 9;                      /* sentinel (>8) */
    while (x > 0) {
        uint8_t d = (uint8_t)(x % 9);      /* least significant digit */
        if (d == 1 && prev == 3)          /* found 1 followed by 3 */
            return true;
        prev = d;
        x /= 9;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t k = 1;
    for (;; ++k) {
        if (has13(k) && !has13(k + 1)) {
            ++count;
            if (count == n)
                return k;
        }
    }
}

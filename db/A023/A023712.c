/*
 * A023712  Numbers with exactly 3 1's in their base 4 expansion.
 * https://oeis.org/A023712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 8838 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A023712: numbers with exactly 3 ones in base‑4 */

#include <stdbool.h>

/* Helper: return true iff val has exactly three digits equal to 1 in its base‑4 expansion */
static bool three_ones(seq_t val) {
    idx_t cnt = 0;
    while (val > 0) {
        if ((val & 3) == 1) {          /* last base‑4 digit: 0…3  -> use last two bits */
            ++cnt;
            if (cnt > 3) return false; /* early exit if more than 3 ones */
        }
        val >>= 2;                     /* shift to discard processed digit */
    }
    return cnt == 3;
}

/* Main function requested by the specification */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* not used, but keep defined for safety */

    idx_t seen = 0;
    seq_t x = 0;

    while (seen < n) {
        ++x;
        if (three_ones(x))
            ++seen;
    }
    return x;
}

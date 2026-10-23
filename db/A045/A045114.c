/*
 * A045114  Numbers whose base-4 representation contains exactly one 1 and no
 *          3's.
 * https://oeis.org/A045114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A045114: numbers whose base‑4 representation contains exactly one 1 and no 3's. */
/* Computed by a straightforward brute‑force search. */

#include <stdbool.h>

/* Test whether x satisfies the digit condition */
static bool cond(seq_t x)
{
    bool seen_one = false;
    while (x) {
        unsigned d = (unsigned)(x % 4);
        if (d == 3) return false;
        if (d == 1) {
            if (seen_one) return false;
            seen_one = true;
        }
        x /= 4;
    }
    return seen_one;
}

/* External linkage as requested */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but harmless */

    idx_t count = 0;
    seq_t cand = 1;
    for (;;) {
        if (cond(cand)) {
            count++;
            if (count == n) return cand;
        }
        cand++;
    }
}

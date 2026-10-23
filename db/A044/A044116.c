/*
 * A044116  Numbers n such that string 2,2 occurs in the base 6 representation
 *          of n but not of n-1.
 * https://oeis.org/A044116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044116: a(n) is the n-th integer m such that the base‑6 representation of m
 * contains the substring “22” but that of m‑1 does not.
 * The function is evaluated by a linear search with base‑6 digit
 * conversion for each tested integer. */
#include <stdbool.h>

static inline bool has_22(seq_t v) {
    if (v == 0) return false;
    seq_t x = v;
    seq_t prev = 6;                 /* something not 0‑5 */
    while (x != 0) {
        seq_t d = x % 6;
        if (d == 2 && prev == 2) return true;
        prev = d;
        x /= 6;
    }
    return false;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1, behaviour for n<1 irrelevant */
    seq_t cur = 0;
    idx_t count = 0;
    bool prev_has = false;

    while (count < n) {
        cur++;                     /* move to next integer */
        bool has = has_22(cur);
        if (has && !prev_has) {
            count++;
            if (count == n) return cur;
        }
        prev_has = has;
    }
    return 0;                      /* should never reach here for n>=1 */
}

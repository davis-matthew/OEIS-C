/*
 * A043844  Numbers n such that number of runs in the base 4 representation of
 *          n is congruent to 1 mod 7.
 * https://oeis.org/A043844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers n such that the number of runs in the base‑4
 *            representation of n is congruent to 1 modulo 7
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 905 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Helper: number of runs modulo 7 in the base‑4 representation of m. */
static inline int runs_mod7(idx_t m) {
    int r = 1;                     /* first run */
    int prev = (int)(m % 4);
    m /= 4;
    while (m > 0) {
        int d = (int)(m % 4);
        if (d != prev) {
            r++;
            if (r >= 7) r -= 7;    /* keep modulo 7 */
        }
        prev = d;
        m /= 4;
    }
    return r;      /* value in 0..6 */
}

/* Main sequence function */
seq_t a(idx_t n) {
    idx_t candidate = 1;   /* smallest positive integer */
    idx_t count = 0;       /* number of accepted terms found so far */
    while (1) {
        if (runs_mod7(candidate) == 1) {
            count++;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
        candidate++;
    }
}

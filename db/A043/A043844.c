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
 * Helpers:   runs_mod7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/runs_mod7.h"

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

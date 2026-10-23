/*
 * A043743  Numbers whose number of runs in the base-2 representation is
 *          congruent to 4 mod 7.
 * https://oeis.org/A043743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose number of runs in base‑2 representation is congruent to 4 mod 7 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t val = 0;
    while (found < n) {
        val += 1;                 /* candidates start from 1 */
        seq_t x = val;
        int runs = 0;
        while (x != 0) {
            seq_t lowbit = x & 1; /* current least significant bit */
            runs += 1;            /* start a new run */
            while (x != 0 && (x & 1) == lowbit) {
                x >>= 1;          /* skip this run */
            }
        }
        if (runs % 7 == 4) {
            ++found;
            if (found == n) {
                return val;
            }
        }
    }
    return 0; /* unreachable */
}

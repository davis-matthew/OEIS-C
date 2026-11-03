/*
 * A043725  Numbers whose number of runs in the base-2 representation is
 *          congruent to 1 mod 4.
 * https://oeis.org/A043725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A043725 – numbers whose number of binary runs is ≡ 1 (mod 4)
 *            (searched)
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 937 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Count the number of runs in the binary representation of x (x>0). */
static inline bool has_run_count_1_mod4(idx_t num)
{
    seq_t x = (seq_t)num;
    /* Since x>0, the first run always starts with the least significant bit. */
    seq_t prev_bit = x & 1;
    seq_t run_count = 1;
    x >>= 1;
    while (x != 0) {
        seq_t bit = x & 1;
        if (bit != prev_bit) {
            ++run_count;
            prev_bit = bit;
        }
        x >>= 1;
    }
    return (run_count & 3) == 1;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    idx_t num    = 1;

    /* Iterate over integers, collecting those with run count ≡ 1 (mod 4). */
    for (;; ++num) {
        if (has_run_count_1_mod4(num)) {
            ++found;
            if (found == n) {
                return (seq_t)num;
            }
        }
    }
}

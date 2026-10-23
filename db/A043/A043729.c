/*
 * A043729  Numbers whose number of runs in their base-2 representation is
 *          congruent to 1 mod 5.
 * https://oeis.org/A043729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose number of runs in their base‑2 representation is
 *            congruent to 1 mod 5
 * Range:     u8 n <= 36, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count runs in the binary representation of x (x > 0). */
static inline seq_t count_runs(seq_t x)
{
    /* x must be positive; for 0 we would return 1 run */
    seq_t runs = 0;
    seq_t prev_bit = x & 1ULL;   /* initial bit */
    while (x) {
        seq_t bit = x & 1ULL;
        if (bit != prev_bit) {
            runs++;
            prev_bit = bit;
        }
        x >>= 1;
    }
    /* Count the final run */
    runs++;
    return runs;
}

seq_t a(idx_t n)
{
    seq_t candidate = 1;  /* start from 1 */
    idx_t found = 0;

    /* Linear search until we find the nth valid number */
    while (found < n) {
        if ((count_runs(candidate) % 5) == 1) {
            found++;
            if (found == n) {
                return candidate;
            }
        }
        candidate++;
    }

    /* Control never reaches here */
    return (seq_t)0;
}

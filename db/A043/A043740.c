/*
 * A043740  Numbers whose number of runs in the base-2 representation is
 *          congruent to 1 mod 7.
 * https://oeis.org/A043740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers whose number of runs in the base-2 representation is
 *            congruent to 1 mod 7
 * Range:     u8 n <= 9, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static idx_t run_count(seq_t x)
{
    idx_t rc = 0;
    int prev = -1;
    while (x != 0) {
        int bit = (int)(x & 1);
        if (bit != prev) {
            rc++;
            prev = bit;
        }
        x >>= 1;
    }
    return rc;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t found = 0;
    seq_t candidate = 1;
    seq_t result = 0;

    while (found < n) {
        if (run_count(candidate) % 7 == 1) {
            found++;
            if (found == n) {
                result = candidate;
                break;
            }
        }
        candidate++;
    }
    return result;
}

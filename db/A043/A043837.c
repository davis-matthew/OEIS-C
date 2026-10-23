/*
 * A043837  Numbers k such that number of runs in the base 4 representation of
 *          k is congruent to 0 mod 6.
 * https://oeis.org/A043837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer k such that the number of runs in the
 *            base‑4 representation of k is congruent to 0 modulo 6
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Count runs in base‑4 representation of x */
static unsigned int base4_runs(seq_t x)
{
    if (x == 0)
        return 1;                     /* representation "0" has one run */

    unsigned int runs = 1;
    unsigned int prev = (unsigned int)(x % 4);
    x /= 4;

    while (x != 0) {
        unsigned int d = (unsigned int)(x % 4);
        if (d != prev) {
            ++runs;
            prev = d;
        }
        x /= 4;
    }
    return runs;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    idx_t k = 0;

    while (found < n) {
        seq_t val = (seq_t)k;
        if (base4_runs(val) % 6 == 0) {
            ++found;
            if (found == n)
                return val;
        }
        ++k;
    }

    /* Unreachable for valid n */
    return 0;
}

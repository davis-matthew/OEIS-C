/*
 * A043723  Numbers whose number of runs in their base-2 representation is
 *          congruent to 2 mod 3.
 * https://oeis.org/A043723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer whose binary representation has number of runs ≡ 2 (mod 3). */
#include <stdint.h>
#include <stdbool.h>

/* Count the number of runs in the binary representation of x. */
static inline int run_count(seq_t x)
{
    int count = 0;
    int prev = 0;
    bool first = true;
    while (x)
    {
        int bit = (int)(x & 1);
        if (first) {
            count = 1;
            prev = bit;
            first = false;
        } else if (bit != prev) {
            ++count;
            prev = bit;
        }
        x >>= 1;
    }
    return count;
}

/* a(n) : n-th positive integer with binary run count ≡ 2 (mod 3). */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t cand = 1;
    while (found < n) {
        if (run_count(cand) % 3 == 2) {
            ++found;
            if (found == n)
                return cand;
        }
        ++cand;
    }
    /* Unreachable because we always find a valid candidate before n is reached. */
    return 0;
}

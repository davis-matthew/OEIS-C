/*
 * A043803  Number of runs in the base 3 representation of n is congruent to 5
 *          mod 8.
 * https://oeis.org/A043803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043803
   a(n) = the n‑th positive integer whose number of runs in its base‑3
   representation is congruent to 5 modulo 8. */
#include <stdint.h>

/* Count the number of runs in the base‑3 representation of x. */
static inline uint64_t runs_in_base3(uint64_t x)
{
    uint64_t runs = 0;
    int prev = -1;
    while (x > 0) {
        int d = (int)(x % 3);
        if (d != prev) {
            runs++;
            prev = d;
        }
        x /= 3;
    }
    return runs;
}

/* Return a(n) as defined above. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t candidate = 1;

    while (count < n) {
        if (runs_in_base3(candidate) % 8 == 5) {
            count++;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate++;
    }

    /* Should never reach here. */
    return (seq_t)0;
}

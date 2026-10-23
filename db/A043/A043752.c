/*
 * A043752  Numbers n such that number of runs in the base 2 representation of
 *          n is congruent to 6 mod 8.
 * https://oeis.org/A043752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043752: Numbers n such that the number of runs in the binary
 * representation of n is congruent to 6 (mod 8).  The function
 * returns the n‑th such number for n >= 1 (zero is undefined). */

#include <stdbool.h>

/* Count runs in the binary representation of x (no leading zeros).  */
static inline int count_runs(seq_t x)
{
    if (x == 0)
        return 0;

    int runs = 1;            /* the first run: most significant bit */
    seq_t prev = x & 1;      /* least significant bit */
    x >>= 1;
    while (x > 0) {
        seq_t bit = x & 1;
        if (bit != prev) {
            ++runs;
            prev = bit;
        }
        x >>= 1;
    }
    return runs;
}

/* Return the n‑th term of the sequence (a(1) = first term).  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n = 0; harmless */

    seq_t candidate = 1;   /* start checking from 1 */
    idx_t found = 0;       /* how many qualifying numbers found */

    while (true) {
        if (count_runs(candidate) % 8 == 6) {
            ++found;
            if (found == n)
                return candidate;
        }
        ++candidate;
    }
}

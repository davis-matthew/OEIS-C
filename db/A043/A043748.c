/*
 * A043748  Numbers n such that number of runs in the base 2 representation of
 *          n is congruent to 2 mod 8.
 * https://oeis.org/A043748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Calculate the OEIS sequence A043748.
 *
 * The sequence lists numbers n such that the number of runs in the
 * binary representation of n is congruent to 2 modulo 8.
 *
 * This implementation scans natural numbers, computes the run count
 * modulo 8 for each, and returns the n‑th qualifying number.
 *
 * The algorithm is O(N) in the value of the n‑th term; for
 * n≈10⁴ the search space is ≈8·n, which is trivial for both 64‑bit
 * and arbitrary‑precision builds.
 */
#include <stdbool.h>

static inline int runs_mod8(seq_t x)
{
    int mod8 = 0;          /* current run count modulo 8   */
    seq_t prev = 0;        /* previous bit (0 or 1)       */
    bool seen = false;     /* have we seen a bit yet?     */

    while (x != 0) {
        seq_t bit = x & 1; /* lowest bit of x             */
        if (!seen || bit != prev) {
            mod8 = (mod8 + 1) & 7; /* increment modulo 8    */
            prev = bit;
            seen = true;
        }
        x >>= 1;
    }
    return mod8;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is 1‑indexed; 0 is undefined */

    idx_t found = 0;
    seq_t cur = 1;

    for (;; cur++) {
        if (runs_mod8(cur) == 2) {
            ++found;
            if (found == n) {
                return cur;
            }
        }
    }
}

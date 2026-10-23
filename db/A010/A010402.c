/*
 * A010402  Squares mod 41.
 * https://oeis.org/A010402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the nth element (1‑based) of the sorted set of quadratic
 *            residues modulo 41. This set consists of all values n² mod 41
 *            for n = 0…40, with duplicates removed and sorted ascending
 * Range:     u8 n <= 21, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the n-th element of the sorted quadratic residues modulo 41.
   For n outside the valid range [1,21] the function returns 0. */
seq_t a(idx_t n)
{
    /* The set has exactly (41+1)/2 = 21 distinct residues. */
    static seq_t residues[21];
    static bool initialized = false;
    if (!initialized) {
        /* Compute the residues once. */
        bool seen[41] = {0};
        for (idx_t i = 0; i <= 40; ++i) {
            seq_t sq = (seq_t)i * (seq_t)i % (seq_t)41;
            /* sq is guaranteed to be in 0…40 */
            seen[(int)sq] = true;
        }
        idx_t k = 0;
        for (idx_t m = 0; m <= 40; ++m) {
            if (seen[(int)m]) {
                residues[k++] = (seq_t)m;
            }
        }
        initialized = true;
    }
    if (n < 1 || n > 21) {
        return (seq_t)0;
    }
    return residues[n - 1];
}

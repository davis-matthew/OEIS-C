/*
 * A010425  Squares mod 64.
 * https://oeis.org/A010425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the n‑th smallest residue of k^2 modulo 64, for k ≥ 0
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MOD 64

seq_t a(idx_t n)
{
    /* Static storage for the sorted list of residues and their count. */
    static seq_t residues[11 + 1];    /* truly 12 residues */
    static idx_t count = 0;
    /* Build the list only once. */
    if (count == 0) {
        bool seen[MOD] = { false };
        /* Mark all residues that occur as squares modulo 64. */
        for (idx_t i = 0; i < MOD; ++i) {
            seq_t r = (seq_t)i * (seq_t)i % (seq_t)MOD;
            seen[(unsigned int)r] = true;
        }
        /* Collect them in ascending order. */
        idx_t pos = 0;
        for (unsigned int r = 0; r < MOD; ++r) {
            if (seen[r]) {
                residues[pos++] = (seq_t)r;
            }
        }
        count = pos;   /* should be 12 */
    }

    /* n is 1‑based; return 0 for out‑of‑range queries. */
    if (n < 1 || n > count) {
        return (seq_t)0;
    }
    return residues[(idx_t)n - 1];
}

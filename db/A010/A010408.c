/*
 * A010408  Squares mod 47.
 * https://oeis.org/A010408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 47*⌊(n-1)/24⌋ + r_{(n-1)%24}, where r_k (k=0..23) are the
 *            quadratic residues modulo 47 sorted in increasing order
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t residues[24];
    static bool initialized = false;
    if (!initialized) {
        bool present[47] = { false };
        /* compute residues of i^2 modulo 47, i=0..46 */
        for (idx_t i = 0; i < 47; ++i) {
            seq_t s   = (seq_t)i;
            seq_t sq  = (s * s) % (seq_t)47;
            present[(size_t)sq] = true;
        }
        /* fill sorted residue array */
        size_t idx = 0;
        for (size_t r = 0; r < 47 && idx < 24; ++r) {
            if (present[r]) {
                residues[idx++] = (seq_t)r;
            }
        }
        initialized = true;
    }

    if (n < 1) {
        /* Undefined for indices below the offset; return 0 as a safe default. */
        return (seq_t)0;
    }

    /* index within the infinite sequence */
    idx_t k   = n - 1;           /* zero-based index */
    idx_t blk = k / 24;          /* which block of 47s */
    idx_t pos = k % 24;          /* position inside the block */

    seq_t block_offset = (seq_t)blk * (seq_t)47;
    return block_offset + residues[(size_t)pos];
}

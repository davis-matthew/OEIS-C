/*
 * A010426  Squares mod 65.
 * https://oeis.org/A010426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* This implementation returns the nth integer that is a quadratic residue modulo 65,
   sorted in increasing order.  
   The residues are found by squaring each integer 0…64 and selecting the distinct
   results modulo 65.  The sequence is then
       a(n) = 65 * floor((n-1)/21) + residues[(n-1)%21]
   where residues[0…20] are the sorted residues
   {0,1,4,9,10,14,16,25,26,29,30,35,36,39,40,49,51,55,56,61,64}. */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n < 1, safe default */

    /* One-time computation of the quadratic residues modulo 65. */
    static bool initialized = false;
    static seq_t residues[21];
    const idx_t RES_COUNT = 21;

    if (!initialized) {
        bool seen[65] = { false };
        /* Mark each residue that occurs. */
        for (idx_t i = 0; i < 65; ++i) {
            seq_t sq = (seq_t)i * (seq_t)i % (seq_t)65;
            seen[(idx_t)sq] = true;
        }
        /* Store residues in increasing order. */
        idx_t idx = 0;
        for (idx_t v = 0; v < 65; ++v) {
            if (seen[v])
                residues[idx++] = (seq_t)v;
        }
        initialized = true;
    }

    /* Compute block and position within the block. */
    idx_t block = (n - 1) / RES_COUNT;
    idx_t pos   = (n - 1) % RES_COUNT;

    return ((seq_t)block) * ((seq_t)65) + residues[pos];
}

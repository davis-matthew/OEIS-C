/*
 * A010458  Squares mod 97.
 * https://oeis.org/A010458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n‑th smallest quadratic residue modulo 97
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (the set {i*i mod 97 | 0 <= i < 97}) */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The sequence contains the 49 distinct quadratic residues mod 97.
       We compute them once on the first call. */
    static seq_t residues[49];
    static bool initialized = false;

    if (!initialized) {
        /* temporary table of which residues have been seen */
        uint8_t seen[97] = {0};
        idx_t i, count = 0;

        /* compute residues i*i mod 97 for i = 0 … 96 */
        for (i = 0; i < 97; ++i) {
            seq_t r = (seq_t)i * (seq_t)i % (seq_t)97;
            seen[(uint8_t)r] = 1;          /* r is always < 97 */
        }

        /* collect the distinct residues into the array */
        for (i = 0; i < 97; ++i) {
            if (seen[i]) {
                residues[count++] = (seq_t)i;
            }
        }

        /* sort the residues ascending (count == 49) */
        for (i = 1; i < count; ++i) {
            seq_t key = residues[i];
            idx_t j = i;
            while (j > 0 && residues[j - 1] > key) {
                residues[j] = residues[j - 1];
                --j;
            }
            residues[j] = key;
        }

        initialized = true;
    }

    if (n == 0 || n > 49) {
        /* sequence is only defined for 1 <= n <= 49 */
        return (seq_t)0;
    }

    return residues[n - 1];
}

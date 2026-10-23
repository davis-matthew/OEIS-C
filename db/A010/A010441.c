/*
 * A010441  Squares mod 80.
 * https://oeis.org/A010441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the nth unique quadratic residue modulo 80, sorted in
 *            increasing order
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Memoized sorted list of quadratic residues modulo 80. */
    static seq_t residues[80];
    static idx_t count = 0;
    static bool initialized = false;
    static bool seen[80] = {0};

    if (!initialized) {
        for (idx_t i = 0; i < 80; ++i) {
            seq_t r = ((seq_t)i * (seq_t)i) % (seq_t)80;
            if (!seen[(unsigned int)r]) {
                seen[(unsigned int)r] = true;
                residues[count++] = r;
            }
        }
        /* Sort the unique residues in ascending order. */
        for (idx_t i = 1; i < count; ++i) {
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

    if (n == 0 || n > count)
        return 0;  /* undefined for out‑of‑range indices */

    return residues[n - 1];
}

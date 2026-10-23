/*
 * A010460  Squares mod 99.
 * https://oeis.org/A010460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the n-th distinct quadratic residue modulo 99, sorted
 *            ascending
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Cache the sorted list of distinct residues modulo 99. */
    static int initialized = 0;
    static seq_t sorted[24];
    static size_t nvals = 0;

    if (!initialized) {
        unsigned char seen[99] = {0};
        seq_t temp[99];
        size_t count = 0;

        /* Compute the set of all squares modulo 99 and keep unique ones. */
        for (idx_t k = 0; k < 99; ++k) {
            seq_t r = ((seq_t)k * (seq_t)k) % (seq_t)99;
            unsigned int ri = (unsigned int)r;   /* 0 <= ri < 99 */
            if (!seen[ri]) {
                seen[ri] = 1;
                temp[count++] = r;
            }
        }

        /* Sort the unique residues in ascending order. */
        for (size_t i = 1; i < count; ++i) {
            seq_t key = temp[i];
            size_t j = i;
            while (j > 0 && temp[j - 1] > key) {
                temp[j] = temp[j - 1];
                --j;
            }
            temp[j] = key;
        }

        /* Copy the sorted residues into the cache. */
        for (size_t i = 0; i < count; ++i) {
            sorted[i] = temp[i];
        }
        nvals = count;
        initialized = 1;
    }

    if (n >= 1 && n <= nvals) {
        return sorted[(size_t)(n - 1)];
    } else {
        /* Undefined for n > number of residues; return 0 as a safe default. */
        return (seq_t)0;
    }
}

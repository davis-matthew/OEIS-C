/*
 * A010379  Squares mod 17.
 * https://oeis.org/A010379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = sorted residues of squares modulo 17 repeated with period
 *            9
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of the 9 distinct values of i^2 mod 17 (i = 0..16),
   arranged in ascending order and then repeated periodically. */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* period = 9, the number of distinct square residues modulo 17 */
    static seq_t residues[9];
    static int built = 0;

    if (!built) {
        bool seen[17] = {false};
        size_t cnt = 0;

        /* Collect the distinct residues of i^2 mod 17 */
        for (idx_t i = 0; i < 17; ++i) {
            seq_t r = ((seq_t)i * (seq_t)i) % (seq_t)17;
            if (!seen[(size_t)r]) {
                seen[(size_t)r] = true;
                residues[cnt++] = r;
            }
        }

        /* Sort the residues ascending */
        for (size_t i = 1; i < cnt; ++i) {
            seq_t key = residues[i];
            size_t j = i;
            while (j > 0 && residues[j - 1] > key) {
                residues[j] = residues[j - 1];
                --j;
            }
            residues[j] = key;
        }

        built = 1;
    }

    if (n == 0)
        return 0;           /* undefined offset, safe return */

    size_t period = 9;
    size_t idx = (size_t)((n - 1) % period);
    return residues[idx];
}

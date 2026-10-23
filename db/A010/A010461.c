/*
 * A010461  Squares mod 100.
 * https://oeis.org/A010461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th smallest quadratic residue modulo 100
 * Range:     u8 n <= 22, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static table of the 22 residues, lazily computed */
    static seq_t residues[22];
    static bool used[100];
    static int count = 0;

    if (count == 0)
    {
        /* compute all residues k^2 mod 100 for 0 <= k < 100 */
        for (idx_t k = 0; k < 100; ++k)
        {
            seq_t r = ((seq_t)k * (seq_t)k) % (seq_t)100;
            used[(unsigned int)r] = true;
        }

        /* collect them into a sorted array */
        int pos = 0;
        for (unsigned int r = 0; r < 100; ++r)
        {
            if (used[r])
            {
                residues[pos++] = r;
            }
        }
        count = pos;   /* should be 22 for modulo 100 */
    }

    if (n == 0 || n > (idx_t)count)
        return (seq_t)0;    /* undefined beyond the sequence length */

    return residues[(int)(n - 1)];
}

/*
 * A028759  Nonsquares mod 46.
 * https://oeis.org/A028759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 46*floor((n-1)/22) + r((n-1)%22) where r(k) is the k‑th
 *            nonsquare residue modulo 46 (0‑based)
 * Range:     u8 n <= 22, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* tables are built only once */
    static bool sq[46];
    static int  ns[22];
    static bool init = false;
    if (!init)
    {
        /* mark all quadratic residues modulo 46 */
        for (idx_t x = 0; x < 46; ++x)
            sq[(x * x) % 46] = true;

        /* collect the residues that are NOT quadratic residues */
        int idx = 0;
        for (int r = 0; r < 46; ++r)
        {
            if (!sq[r])
                ns[idx++] = r;          /* idx will become 22 */
        }
        init = true;
    }

    idx_t block = (n - 1) / 22;          /* how many full periods of 46 */
    idx_t pos   = (n - 1) % 22;          /* position inside the period   */
    seq_t value = (seq_t)block * (seq_t)46 + (seq_t)ns[(int)pos];
    return value;
}

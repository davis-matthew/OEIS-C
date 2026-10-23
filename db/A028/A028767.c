/*
 * A028767  Nonsquares mod 54.
 * https://oeis.org/A028767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th positive integer that is not a quadratic residue
   modulo 54.  The sequence a(n) is given by */
 /* a(n) = 54 * floor((n-1)/32) + r[(n-1) mod 32]            */
 /* where r[0..31] are the 32 distinct residues mod 54 that are
    not quadratic residues, listed in increasing order.      */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n = 0 is outside the defined range; return 0 for safety. */
    if (n == 0)
        return 0;

    /* Lazy initialisation of the non‑residue residues array. */
    static bool   init_ok     = false;
    static seq_t  residues[32];

    if (!init_ok) {
        /* Mark all quadratic residues modulo 54. */
        static bool is_square[54] = {0};
        for (idx_t x = 0; x < 54; ++x) {
            idx_t r = (x * x) % 54;
            is_square[r] = true;
        }

        /* Collect the non‑residues in ascending order. */
        unsigned k = 0;
        for (idx_t r = 0; r < 54; ++r) {
            if (!is_square[r]) {
                residues[k++] = (seq_t)r;
            }
        }
        /* k must be 32; ignore the value in the AMP. */
        (void)k;
        init_ok = true;
    }

    /* Compute the block index and residue index. */
    idx_t i  = (n - 1) % 32;      /* residue position 0..31 */
    idx_t q  = (n - 1) / 32;      /* how many full 54‑blocks before */

    /* Final value: 54*q + residue[i] */
    seq_t val = ((seq_t)q) * (seq_t)54 + residues[i];
    return val;
}

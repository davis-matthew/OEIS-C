/*
 * A010427  Squares mod 66.
 * https://oeis.org/A010427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n‑th smallest quadratic residue modulo 66 (the
 *            residues r such that ∃t: t² ≡ r (mod 66).)
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Precompute the sorted list of residues only once. */
    static seq_t residues[66];
    static size_t len = 0;
    static bool init = false;

    if (!init) {
        bool present[66] = { false };
        for (idx_t t = 0; t < 66; ++t) {
            seq_t r = ((seq_t)t * (seq_t)t) % ((seq_t)66);
            present[(unsigned int)r] = true;
        }
        size_t pos = 0;
        for (unsigned int i = 0; i < 66; ++i)
            if (present[i]) residues[pos++] = (seq_t)i;
        len = pos;
        init = true;
    }

    if (n < 1 || n > (idx_t)len)
        return (seq_t)0;            /* undefined for out‑of‑range n */

    return residues[(size_t)(n - 1)];
}

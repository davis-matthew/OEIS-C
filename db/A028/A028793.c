/*
 * A028793  Nonsquares mod 80.
 * https://oeis.org/A028793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 80*floor((n-1)/68) + r[(n-1)%68], where r[] are the
 *            non‑quadratic residues modulo 80 in increasing order
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>


seq_t a(idx_t n)
{
    /* Cached residues that are NOT quadratic residues modulo 80. */
    static unsigned char residues[68];
    static size_t residue_len = 0;
    static bool ready = false;

    if (!ready) {
        /* Mark quadratic residues modulo 80. */
        bool is_res[80] = {false};
        for (size_t j = 0; j < 80; ++j) {
            seq_t sq = ((seq_t)j * (seq_t)j) % 80;
            is_res[(size_t)(unsigned long long)sq] = true;
        }

        /* Collect the non‑residues in increasing order. */
        for (size_t j = 0; j < 80; ++j) {
            if (!is_res[j]) {
                residues[residue_len++] = (unsigned char)j;
            }
        }
        ready = true;
    }

    /* Compute zero‑based index of the term. */
    idx_t idx   = n - 1;
    idx_t block = idx / 68;           /* number of complete cycles of 80 */
    size_t offset = (size_t)(idx % 68); /* position within the current cycle */

    /* Compose the nth term. */
    return ((seq_t)block) * 80 + (seq_t)residues[offset];
}

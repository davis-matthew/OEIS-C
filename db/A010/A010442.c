/*
 * A010442  Squares mod 81.
 * https://oeis.org/A010442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A010442 – Squares modulo 81
   Formula: a(n) = (n-1)/R * 81 + residues[(n-1)%R]
   where R = 29 is the number of distinct quadratic residues modulo 81
   and residues[] is the increasing list of those residues.
   The sequence is the infinite repetition of these residues. */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Pre‑computed list of quadratic residues modulo 81.
       Values are 0 ≤ r < 81.  R = 29 residues in total. */
    static seq_t residues[81];
    static size_t R = 0;
    static bool initialized = false;

    if (!initialized) {
        unsigned char present[81] = {0};
        /* Mark all residues obtained by squaring 0 … 80. */
        for (idx_t i = 0; i < 81; ++i) {
            seq_t s = (seq_t)i * (seq_t)i;
            present[(int)(s % (seq_t)81)] = 1;
        }
        /* Store the present residues in ascending order. */
        size_t pos = 0;
        for (idx_t r = 0; r < 81; ++r) {
            if (present[(int)r]) {
                residues[pos++] = (seq_t)r;
            }
        }
        R = pos;          /* Should be 29 for modulus 81. */
        initialized = true;
    }

    idx_t idx = n - 1;                    /* zero‑based position */
    idx_t k   = idx / (idx_t)R;           /* complete blocks of R residues */
    idx_t ri  = idx % (idx_t)R;           /* position inside the block */

    return residues[ri] + (seq_t)k * (seq_t)81;
}

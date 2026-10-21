/*
 * A056031  Numbers k such that k^22 == 1 (mod 23^2).
 * https://oeis.org/A056031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2725, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th term of OEIS A056031.
   The sequence consists of all positive integers k for which
   k^22 ≡ 1 (mod 23²).  The solutions modulo 23² form a
   subgroup of size 22 in the multiplicative group of units
   modulo 23².  Thus the solutions are exactly the 22
   residues r (1 ≤ r < 23²) with r^22 ≡ 1 (mod 529), and
   every element of the sequence is one of those residues
   increased by a multiple of 23² = 529.

   The recurrence supplied in the OEIS entry is equivalent to
   a(n) = a(n–22) + 529 (n > 22) with the initial 22 terms
   being the sorted solutions modulo 529.
   The following implementation computes the 22 residues
   on first use by brute‑force search; thereafter each term
   is obtained in O(1).

   The function is fully generic for the build‑specific
   integer type seq_t and index type idx_t.  It does not
   perform any I/O and no external files are read.
*/

#include <stdbool.h>
#include "../utils/mod_pow.h"   /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

static seq_t base_residues[22];
static bool initialized = false;

static void
init_base_residues(void)
{
    if (initialized) return;
    const seq_t mod = (seq_t)529;          /* 23² */
    idx_t count = 0;
    for (idx_t k = 1; k < 529; ++k) {
        if (mod_pow((seq_t)k, 22, mod) == (seq_t)1) {
            base_residues[count++] = (seq_t)k;
        }
    }
    /* According to the theory there must be exactly 22 solutions. */
    initialized = true;
}

/* a(idx_t n) returns the n-th term (n ≥ 1) of the sequence. */
seq_t
a(idx_t n)
{
    init_base_residues();

    /* n–1 = 22·q + r, with r in [0,21], q ≥ 0 */
    idx_t r = (n - 1) % 22;          /* 0‑based index of the base residue */
    idx_t q = (n - 1) / 22;          /* number of completed periods of 23² */

    const seq_t base = base_residues[r];
    const seq_t step = (seq_t)529;
    return base + q * step;
}

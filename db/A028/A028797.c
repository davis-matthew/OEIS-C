/*
 * A028797  Nonsquares mod 84.
 * https://oeis.org/A028797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth non‑square modulo 84 by enumerating integers
 * and testing if their residue modulo 84 is a quadratic
 * residue.  The sequence contains all positive integers that
 * are NOT quadratic residues modulo 84. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Precompute the set of quadratic residues modulo 84 */
    static bool init = false;
    static bool sq[84];
    if (!init) {
        for (int i = 0; i < 84; ++i) sq[i] = false;
        for (int x = 0; x < 84; ++x) {
            int r = (x * x) % 84;
            sq[r] = true;
        }
        init = true;
    }

    seq_t count = 0;      /* number of non‑residues seen so far */
    seq_t cur   = 1;      /* candidate integer */

    while (count < n) {
        seq_t r = cur % (seq_t)84;          /* residue modulo 84 */
        if (!sq[(int)r]) {                  /* not a quadratic residue */
            ++count;
            if (count == n) return cur;     /* nth term found */
        }
        ++cur;
    }
    /* Should never reach here */
    return cur;
}

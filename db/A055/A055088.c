/*
 * A055088  Triangle of generalized Legendre symbols L(a/b) read by rows, with
 *          1's for quadratic residues and 0's for quadratic non-residues.
 * https://oeis.org/A055088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A055088: a(n) = 1 if ∃c with c² ≡ a (mod b) else 0,
   where a and b are obtained from the triangle indexing:
   row r (1‑based) has modulus b = r + 1 and contains r entries
   for a = 1 … r.  The sequence is read row‑wise. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* offset 1: ignore n = 0 */

    /* Find the row r such that T_{r‑1} < n ≤ T_r,  T_r = r(r + 1)/2. */
    idx_t r = 0;
    idx_t t = 0;                    /* current triangular number T_r */
    while (t < n) {
        ++r;
        t += r;                     /* t = r(r + 1)/2 recursively */
    }

    idx_t tprev = t - r;            /* T_{r‑1} */
    idx_t a_col = n - tprev;        /* column index a (1‑based) */
    idx_t b = r + 1;                /* modulus for this row */

    /* Brute‑force check for a quadratic residue modulo b. */
    for (idx_t c = 0; c < b; ++c) {
        if ((c * c) % b == a_col) return (seq_t)1;
    }
    return (seq_t)0;
}

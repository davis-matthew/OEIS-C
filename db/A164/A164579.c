/*
 * A164579  Integer averages of halves of first cubes of natural numbers
 *          (n^3)/2 for some n.
 * https://oeis.org/A164579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n + 1)^2 / 8 for n ≡ 0, 3, 7 (mod 8)
 * Range:     u8 n <= 4, u16 n <= 29, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 6, a(2) = 56, a(3) = 81, … (integer terms only) */

#include <stddef.h>

/* Offsets of allowed residues within each 8‑block: 0 → 3, 1 → 7, 2 → 8 */
static const int offset[3] = { 3, 7, 8 };

/* Return the nth term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n = 0, but safe to return 0 */

    /* --- Map the index n to the corresponding m satisfying the divisibility test --- */
    idx_t blk = (n - 1) / 3;            /* number of complete blocks of 3 allowed terms */
    idx_t r   = (n - 1) % 3;            /* position within the current cycle */
    idx_t m_idx = blk * (idx_t)8 + (idx_t)offset[r];

    /* --- Compute m*(m + 1)^2 / 8 with an early division by 8 to avoid overflow --- */
    seq_t m   = (seq_t)m_idx;
    seq_t p0  = m;                       /* one copy of m          */
    seq_t p1  = m + (seq_t)1;            /* one copy of (m + 1)    */
    seq_t p2  = m + (seq_t)1;            /* second copy of (m + 1) */

    seq_t remaining = (seq_t)8;           /* factor still to divide out */
    seq_t *parts[3] = { &p0, &p1, &p2 };

    for (int i = 0; i < 3 && remaining > 1; ++i) {
        while ((*parts[i] % (seq_t)2 == 0) && remaining > 1) {
            *parts[i] /= (seq_t)2;
            remaining /= (seq_t)2;
        }
    }

    seq_t result = p0 * p1;              /* intermediate product */
    result       = result * p2;          /* final product is the desired term */

    return result;
}

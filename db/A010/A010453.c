/*
 * A010453  Squares mod 92.
 * https://oeis.org/A010453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 92 * floor((n-1)/24) + r_k, where r_k is the k-th
 *            quadratic residue modulo 92 in sorted order (r_1 = 0, r_2 = 1,
 *            ..., r_24 = 85)
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Quadratic residues modulo 92, in increasing order. */
static const seq_t residues[24] = {
    0ULL, 1ULL, 4ULL, 8ULL, 9ULL, 12ULL, 13ULL, 16ULL,
   24ULL, 25ULL, 29ULL, 32ULL, 36ULL, 41ULL, 48ULL, 49ULL,
   52ULL, 64ULL, 69ULL, 72ULL, 73ULL, 77ULL, 81ULL, 85ULL
};

seq_t a(idx_t n)
{
    /* n is 1‑based */
    idx_t idx   = n - 1;          /* zero‑based index */
    idx_t cycle = idx / 24;       /* number of complete residue cycles */
    idx_t pos   = idx % 24;       /* position within the residue cycle */

    return ((seq_t)cycle) * ((seq_t)92) + residues[pos];
}

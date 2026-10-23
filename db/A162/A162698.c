/*
 * A162698  Numbers n such that the incidence matrix of the grid n X n has -1
 *          as eigenvalue.
 * https://oeis.org/A162698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30 * floor((n-1)/10) + res[(n-1)%10] * where res =
 *            {4,5,9,11,14,17,19,23,24,29}. This gives the n‑th * integer n
 *            such that n+1 is divisible by 5 or 6
 * Range:     u8 n <= 85, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const unsigned char residues[10] = {4,5,9,11,14,17,19,23,24,29};

seq_t a(idx_t n)
{
    /* n >= 1 as per sequence definition */
    idx_t q = (n - 1) / 10;   /* number of complete 30‑blocks */
    idx_t r = (n - 1) % 10;   /* position within the block (0‑based) */
    return (seq_t)30 * (seq_t)q + (seq_t)residues[r];
}

/*
 * A028763  Nonsquares mod 50.
 * https://oeis.org/A028763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th positive integer that is not a quadratic residue
 *            modulo 50
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A028763 consists of all positive integers whose value modulo 50
 * is not in the set of quadratic residues modulo 50.
 * There are 22 square residues modulo 50, leaving 28 non‑square residues.
 * The residues in increasing order are:
 *     2,3,5,7,8,10,12,13,15,17,18,20,22,23,27,28,30,32,33,35,37,38,40,42,43,45,47,48
 * Since the pattern repeats every 50, we can compute a(n) in O(1):
 *
 *   let k = (n-1)/28            // number of complete 50‑blocks preceding the answer
 *   let r = (n-1)%28            // index within the 28‑residue list
 *   a(n) = k*50 + non_square_residues[r]
 */

#include <stdint.h>
#include <stdbool.h>

static const unsigned char non_square_residues[28] = {
    2,3,5,7,8,10,12,13,15,17,18,20,22,23,
    27,28,30,32,33,35,37,38,40,42,43,45,47,48
};

seq_t a(idx_t n)
{
    /* n is 1‑based */
    idx_t k = (n - 1) / 28;
    idx_t r = (n - 1) % 28;
    seq_t base = (seq_t)k * 50U;
    return base + (seq_t)non_square_residues[r];
}

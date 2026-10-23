/*
 * A025790  Expansion of 1/((1-x)(1-x^8)(1-x^11)).
 * https://oeis.org/A025790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{⌊n/11⌋} (⌊(n - 11k)/8⌋ + 1) (Number of
 *            non‑negative integer solutions to 8·j + 11·k ≤ n.)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t max_k = n / 11;                /* largest value of k with 11·k ≤ n */
    for (idx_t k = 0; k <= max_k; ++k)   /* iterate over all admissible k */
    {
        idx_t rem   = n - 11 * k;        /* remaining amount after subtracting 11·k */
        idx_t max_j = rem / 8;           /* largest j with 8·j ≤ rem */
        sum += (seq_t)max_j + 1;         /* j runs from 0 to max_j inclusive */
    }
    return sum;
}

/*
 * A194228  Partial sums of A194227.
 * https://oeis.org/A194228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{r∈{3,5,6}, r≤n} [(⌊(n−r)/7⌋+1)*(n−r+1) −
 *            7*⌊(n−r)/7⌋*(⌊(n−r)/7⌋+1)/2]
 * Range:     u8 n <= 34, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence counts the partial sums of the indicator sequence for numbers ≡ 3,5,6 (mod 7). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial cases quickly */
    if (n <= 2)
        return 0;

    const int residues[3] = {3, 5, 6};
    seq_t sum = 0;
    seq_t nn   = (seq_t)n;

    for (int k = 0; k < 3; ++k) {
        idx_t r = residues[k];
        if (n < r)
            continue;                   /* no terms for this residue */

        idx_t m = (n - r) / 7;          /* floor((n-r)/7) */
        seq_t mm = (seq_t)m;

        /* term1 = (m + 1) * (n - r + 1) */
        seq_t term1 = (mm + 1) * (nn - (seq_t)r + 1);

        /* term2 = 7 * m * (m + 1) / 2 */
        seq_t term2 = 7 * mm * (mm + 1) / 2;

        sum += term1 - term2;
    }

    return sum;
}

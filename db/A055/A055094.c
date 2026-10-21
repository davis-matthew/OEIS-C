/*
 * A055094  Binary encoding of quadratic residue set of n. L(1/n) is the most
 *          significant bit, L(n-1/n) is the least significant bit, i.e., the
 *          rows of A055088 interpreted as binary numbers.
 * https://oeis.org/A055094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binary number whose bits from most to least significant
 *            are L(1/n)…L(n-1/n). Computation: compute all quadratic residues
 *            modulo n and assemble bits
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* Allocate array to track residues. */
    size_t m = (size_t)n;
    bool *is_res = (bool *)calloc(m, sizeof(bool));
    if (!is_res)
        return 0;           /* allocation failure → zero */

    /* Mark all residues of squares modulo n. */
    for (idx_t x = 0; x < n; ++x) {
        seq_t sq = ((seq_t)x * (seq_t)x) % (seq_t)n;
        is_res[(size_t)sq] = true;
    }

    /* Assemble the binary number: the most significant bit corresponds to a = 1. */
    seq_t val = 0;
    for (idx_t b = 1; b < n; ++b) {
        val <<= 1;
        if (is_res[(size_t)b])
            val |= 1;
    }

    free(is_res);
    return val;
}

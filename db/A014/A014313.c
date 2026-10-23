/*
 * A014313  Numbers with exactly 5 ones in binary expansion.
 * https://oeis.org/A014313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th smallest integer whose binary expansion has
 *            exactly five 1’s
 * Range:     u8 n <= 21, u16 n <= 4368, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

static seq_t
comb(idx_t n, idx_t k)          /* binomial coefficient C(n,k) */
{
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t
a(idx_t n)                      /* 1‑based index */
{
    if (n == 0) return 0;       /* value below the offset is irrelevant */

    idx_t r = n - 1;            /* rank of the desired combination (0‑based) */
    idx_t pos[5];

    /* Backwards combinatorial number system: determine the 5 bit positions. */
    for (idx_t j = 5; j >= 1; --j) {
        idx_t i = j - 1;        /* smallest i with C(i,j) = 0 when i < j */
        while (comb(i + 1, j) <= (seq_t)r)
            ++i;
        pos[j - 1] = i;        /* store the j‑th position (descending order) */
        r -= (idx_t)comb(i, j);
    }

    /* Build the integer by summing powers of two at the found positions. */
    seq_t val = 0;
    for (idx_t k = 0; k < 5; ++k)
        val += pow2(pos[k]);

    return val;
}

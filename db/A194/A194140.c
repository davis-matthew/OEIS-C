/*
 * A194140  a(n) = Sum_{j=1..n} floor(j*(1+sqrt(3))); n-th partial sum of
 *          Beatty sequence for 1+sqrt(3).
 * https://oeis.org/A194140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=1..n} ⌊j · (1+√3)⌋
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation returns the partial sum of the Beatty sequence for 1+√3. */

#include <stdint.h>
/* seq_t and idx_t are defined by the build, do not define them here. */

static inline unsigned bitlen_seq_t(seq_t x)
{
    unsigned bl = 0;
    while (x > 0) {
        x >>= 1;
        bl++;
    }
    return bl;
}

/* Integer square root: floor(√x) for any seq_t */
static inline seq_t floor_sqrt_seq(seq_t x)
{
    if (x == 0)
        return 0;

    /* upper bound for √x : 1 << ((bitlen(x)+1)/2) */
    unsigned bl = bitlen_seq_t(x);
    seq_t high = (seq_t)1 << ((bl + 1) >> 1);
    seq_t low  = 0;

    while (low < high) {
        seq_t mid = low + ((high - low + 1) >> 1);
        /* Avoid overflow: compare mid with x/mid */
        if (mid > 0 && mid > x / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum_sqrt = 0;
    /* Compute Σ floor(j·√3) */
    for (idx_t j = 1; j <= n; ++j) {
        seq_t jj = (seq_t)j;
        seq_t val = floor_sqrt_seq((seq_t)3 * jj * jj);
        sum_sqrt += val;
    }

    /* Sum of j from 1 to n */
    seq_t sum_j = (seq_t)n * ((seq_t)n + 1) / 2;

    return sum_sqrt + sum_j;
}

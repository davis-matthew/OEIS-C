/*
 * A174426  Denominator of fractions in A171676.
 * https://oeis.org/A174426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = (m*(m+1))^2 where m = ⌊(√(8n+1)+1)/2⌋
 * Range:     u8 n <= 5, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the largest m ≥ 1 such that m*(m-1)/2 ≤ n. */
static idx_t
find_m(idx_t n)
{
    /* The bound 2^32 is sufficient for any 64‑bit n. */
    idx_t low  = 1;
    idx_t high = (idx_t)1u << 32;

    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        /* mid*(mid-1)/2 fits in 128 bits for all mid ≤ 2^32. */
        unsigned __int128 val = (unsigned __int128)mid * (mid - 1) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t
a(idx_t n)
{
    idx_t m   = find_m(n);
    seq_t dm  = (seq_t)m * ((seq_t)m + 1);
    return dm * dm;
}

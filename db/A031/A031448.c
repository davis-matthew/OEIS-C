/*
 * A031448  Numbers whose base-2 representation has one fewer 0's than 1's.
 * https://oeis.org/A031448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth number whose binary representation has one more 1
 *            than 0 (i.e. popcount(x) = (bits(x)+1)/2)
 * Range:     u8 n <= 29, u16 n <= 4707, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n=0, but protect */

    /* Find the binary length L (odd) that contains the nth term. */
    seq_t cumulative = 0;          /* terms of shorter lengths */
    idx_t L = 1;                   /* current odd length */
    while (1) {
        idx_t half   = (L - 1) >> 1;          /* (L-1)/2 = ones in lower bits */
        seq_t count  = binom(L - 1, half);    /* C(L-1, (L-1)/2) */
        if (cumulative + count >= (seq_t)n)   /* target length found */
            break;
        cumulative += count;
        L += 2;                        /* next odd length */
    }

    /* zero-based index within the block of length L */
    seq_t n_minus_cum = (seq_t)n - cumulative;
    idx_t k = (idx_t)(n_minus_cum - 1);  /* idx_t is 64‑bit */
    seq_t k_seq  = k;

    /* Build the number: leading 1 at bit L-1 */
    seq_t num = (seq_t)1;
    num <<= (L - 1);

    /* Remaining ones to place in the lower L-1 bits */
    idx_t ones = (L - 1) >> 1;                /* (L-1)/2 */

    /* Unrank the combination of positions for the remaining ones. */
    for (idx_t pos = 0; pos < L - 1 && ones > 0; ++pos) {
        idx_t remaining_bits_after = (L - 2) - pos;
        seq_t ways = binom(remaining_bits_after, ones);

        /* If the desired index is beyond all combinations with 0 at this bit,
           we must set this bit to 1. */
        if (k_seq >= ways) {
            num |= ((seq_t)1 << (remaining_bits_after));
            k_seq -= ways;
            --ones;
        }
        /* else leave bit 0 */
    }
    return num;
}

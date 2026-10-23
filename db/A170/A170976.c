/*
 * A170976  Expansion of (Product_{i=1..m-1} (1 + x^(4*i-1))) * (1 +
 *          x^(2*m-1)) for m = 4.
 * https://oeis.org/A170976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of subsets of the multiset {3,7,7,11} whose
 *            element sums equal n (indices are counted as distinct)
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Degrees present in the polynomial:
       0,3,7,10,11,14,17,18,21,25,28 with respective coefficients
       1,1,2,2,1,2,1,2,2,1,1. All others up to 28 are zero. */
    if (n > 28) return 0;

    static const idx_t exps[4] = {3, 7, 7, 11};
    seq_t count = 0;

    for (idx_t mask = 0; mask < 16; ++mask) {
        idx_t sum = 0;
        for (int i = 0; i < 4; ++i)
            if (mask & (1u << i))
                sum += exps[i];
        if (sum == n)
            ++count;
    }
    return count;
}

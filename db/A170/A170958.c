/*
 * A170958  Expansion of Product_{i=1..m} (1 + x^(4*i-1)) for m = 5.
 * https://oeis.org/A170958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of subsets of {3, 7, 11, 15, 19} whose exponents
 *            sum to n
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Exponents in the product (1 + x^(4*i-1)) for i = 1..5 */
    const idx_t exps[5] = {3, 7, 11, 15, 19};
    seq_t count = 0;

    /* Enumerate all 2^5 = 32 subsets via bit masks. */
    for (idx_t mask = 0; mask < ((idx_t)1 << 5); ++mask) {
        idx_t sum = 0;
        for (idx_t i = 0; i < 5; ++i) {
            if (mask & ((idx_t)1 << i))
                sum += exps[i];
        }
        if (sum == n)
            ++count;
    }
    return count;
}

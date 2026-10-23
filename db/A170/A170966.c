/*
 * A170966  Expansion of Product_{i=0..m-1} (1 + x^(4*i+1)) for m = 3.
 * https://oeis.org/A170966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 1 if n is a sum of distinct elements of {1,5,9} else 0
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > 15)            /* maximum sum 1+5+9 = 15 */
        return 0;

    const idx_t ex[3] = {1, 5, 9};

    for (int mask = 0; mask < 8; ++mask) {
        idx_t sum = 0;
        if (mask & 1) sum += ex[0];
        if (mask & 2) sum += ex[1];
        if (mask & 4) sum += ex[2];

        if (sum == n)
            return 1;
    }
    return 0;
}

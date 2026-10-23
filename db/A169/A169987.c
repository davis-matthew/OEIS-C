/*
 * A169987  Expansion of Product_{i=0..m-1} (1 + x^(2*i+1)) for m=4.
 * https://oeis.org/A169987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of subsets of {1,3,5,7} with sum n
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > 16u)
        return 0;

    /* The odd parts of the product (1 + x)(1 + x³)(1 + x⁵)(1 + x⁷) */
    static const int odds[4] = {1, 3, 5, 7};
    seq_t count = 0;

    for (int mask = 0; mask < 16; ++mask) {
        int sum = 0;
        if (mask & 1) sum += odds[0];
        if (mask & 2) sum += odds[1];
        if (mask & 4) sum += odds[2];
        if (mask & 8) sum += odds[3];
        if ((idx_t)sum == n)
            ++count;
    }
    return count;
}

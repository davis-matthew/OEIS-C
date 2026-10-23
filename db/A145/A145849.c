/*
 * A145849  a(n) = A145812(2n-1).
 * https://oeis.org/A145849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + ∑_{k≥0} ((n-1) >> k & 1) * 8·4^k
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of numbers of the form 1 plus a sum of distinct powers of 8. */

seq_t a(idx_t n) {
    /* Start with 1.  Each bit of (n-1) selects a power of 8 to add. */
    seq_t val = 1;
    seq_t add = 8;          /* 2^3, the first power of 8. */
    idx_t m = n - 1;

    while (m) {
        if (m & 1)          /* if this bit is set, include the current power of 8. */
            val += add;
        add <<= 2;          /* next power of 8: multiply by 4 (2^2). */
        m >>= 1;            /* move to the next bit. */
    }
    return val;
}

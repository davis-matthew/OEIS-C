/*
 * A084623  Numerator of 2^(n-1)/n.
 * https://oeis.org/A084623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2^(n - v2(n) - 1) where v2(n) is the exponent of 2
 *            dividing n
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t v2_trailing_zeros(idx_t x) {
    idx_t count = 0;
    while ((x & 1) == 0 && x != 0) {
        x >>= 1;
        count++;
    }
    return count;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                  /* Undefined below the offset. */
    idx_t v = v2_trailing_zeros(n);        /* v = v₂(n). */
    idx_t exp = n - 1 - v;                 /* exponent for 2. */
    seq_t result = 1;
    result <<= (unsigned long long)exp;    /* 2^exp in the smallest representation. */
    return result;
}

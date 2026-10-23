/*
 * A020070  a(n) = floor( Gamma(n+7/8)/Gamma(7/8) ).
 * https://oeis.org/A020070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( Gamma(n+7/8)/Gamma(7/8) ) Equivalent to a(n) =
 *            floor( ∏_{k=0}^{n-1} (8k+7) / 8^n ) Since all numerator factors
 *            are odd, the denominator is a power of two. The result is the
 *            product shifted right by 3*n bits (division by 8^n)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;            /* a(0) = 1 */

    /* For small seq_t sizes (≤64 bits) use a 128‑bit intermediate
       to avoid overflow while the final value fits in seq_t. */
    if (sizeof(seq_t) <= 8) {
        __uint128_t prod = 1;
        for (idx_t k = 0; k < n; ++k)
            prod *= (uint64_t)(8 * k + 7);

        /* 8^n = 2^(3n); shift right by 3*n bits.  n is small enough
           here that 3*n ≤ 127, so the shift is defined. */
        prod >>= (3 * n);
        return (seq_t)prod;
    }

    /* For larger seq_t (arbitrary‑precision) just multiply and
       shift right by 3*n bits.  The right‑shift operator is defined
       for the integer types used by the tests. */
    seq_t prod = 1;
    for (idx_t k = 0; k < n; ++k)
        prod *= (seq_t)(8 * k + 7);

    prod >>= (3 * n);
    return prod;
}

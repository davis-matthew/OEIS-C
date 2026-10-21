/*
 * A059867  Number of irreducible representations of the symmetric group S_n
 *          that have odd degree.
 * https://oeis.org/A059867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(∑_{i : (n>>i)&1} i) for n ≥ 1 (a(1)=1)
 * Range:     u8 n <= 25, u16 n <= 93, u32 n <= 493, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* the OEIS offset is 1; for n = 0 we return 0 to avoid UB */
    if (n == 0) return 0;

    /* sum the indices of set bits in n */
    idx_t sum = 0;
    idx_t pos = 0;
    idx_t m = n;

    while (m) {
        if (m & 1ULL) sum += pos;
        m >>= 1;
        ++pos;
    }

    /* compute 2^sum using repeated multiplication to stay within the
       bounds of the unsigned type seq_t without using shift counts
       that may exceed the type width.  For unsigned types overflow
       is defined modulo 2^N, but the tests will only check values
       that fit into seq_t. */
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < sum; ++i) {
        r *= (seq_t)2;
    }

    return r;
}

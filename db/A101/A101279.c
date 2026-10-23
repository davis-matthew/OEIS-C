/*
 * A101279  a(1) = 1; a(2k) = a(k), a(2k+1) = k.
 * https://oeis.org/A101279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 258, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = 1, if n=1
 *  a(2k)   = a(k)
 *  a(2k+1) = k
 *  (equivalently:  a(n) = ((n>>v)-1)/2 + (n==1<<v),  where v = v₂(n) )
 */
seq_t a(idx_t n)
{
    /* handle the trivial case – the sequence is defined for n>=1 only */
    if (n == 0) return 0;

    /* compute the exponent v such that 2^v | n and 2^(v+1) ∤ n */
    idx_t temp = n;
    idx_t v = 0;
    while ((temp & 1) == 0) {
        temp >>= 1;
        ++v;
    }

    /* temp is now n / 2^v (an odd integer) */
    seq_t part1 = ((seq_t)temp - (seq_t)1) / (seq_t)2;           /* (n/2^v - 1)/2 */
    seq_t part2 = (n == ((idx_t)1 << v)) ? (seq_t)1 : (seq_t)0; /* +1 if n is a power of two */

    return part1 + part2;
}

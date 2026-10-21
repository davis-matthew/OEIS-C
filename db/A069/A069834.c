/*
 * A069834  a(n) = n-th reduced triangular number: n*(n+1)/{2^k} where 2^k is
 *          the largest power of 2 that divides product n*(n+1).
 * https://oeis.org/A069834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2^k, where 2^k is the largest power of 2 dividing
 *            n*(n+1)
 * Range:     u8 n <= 15, u16 n <= 360, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1 – a(1) is the first term.                                             */
seq_t a(idx_t n)
{
    /* n*(n+1) is always even for n ≥ 1 */
    seq_t prod = (seq_t)n * ((seq_t)n + 1);

    /* Strip all factors of 2 to obtain the odd part of the product. */
    while ((prod % 2) == 0)
        prod /= 2;

    return prod;
}

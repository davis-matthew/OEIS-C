/*
 * A099545  Odd part of n, modulo 4.
 * https://oeis.org/A099545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (odd part of n) mod 4
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n < 1, but guard against overflow */
    seq_t x = (seq_t)n;              /* cast index to seq_t for arithmetic */
    while ((x & 1) == 0)            /* strip powers of two */
        x >>= 1;
    return x % 4;                    /* remainder modulo 4 (1 or 3) */
}

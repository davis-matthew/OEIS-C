/*
 * A085743  a(n) = T(n^3) - T(n^2), where T() are the triangular numbers
 *          (A000217).
 * https://oeis.org/A085743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*n^2*(n^3 + n^2 + 1)/2
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 45, u64 n <= 1824,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t sn  = (seq_t)n;          /* n           */
    seq_t sn1 = sn - 1;            /* n-1         */
    seq_t sn2 = sn * sn;           /* n^2         */
    seq_t sn3 = sn2 * sn;          /* n^3         */
    seq_t term = sn3 + sn2 + 1;    /* n^3 + n^2 + 1 */

    /* Use 128‑bit intermediate to avoid overflow for large n */
    __uint128_t prod = (__uint128_t)sn1 * (__uint128_t)sn2;
    prod *= (__uint128_t)term;
    prod /= 2;

    return (seq_t)prod;
}

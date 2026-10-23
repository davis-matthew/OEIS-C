/*
 * A038707  a(n) = floor(n*(n+1/2)/2).
 * https://oeis.org/A038707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * (2*n + 1) / 4)
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use 128‑bit arithmetic for the intermediate product to avoid overflow */
    __uint128_t un = n;                         /* promote index to 128‑bit */
    __uint128_t prod = un * ((un << 1) + 1);    /* n * (2*n + 1) */
    prod >>= 2;                                 /* floor division by 4 (>> works as /4 for unsigned) */
    return (seq_t)prod;                         /* cast back to the sequence type */
}

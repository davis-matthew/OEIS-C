/*
 * A212161  Numbers congruent to 6 or 10 mod 17.
 * https://oeis.org/A212161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17 * floor(n/2) + (n % 2 ? 10 : 6)
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t half = n / 2;                      /* floor(n/2) */
    seq_t base  = (seq_t)17 * (seq_t)half;   /* 17 * floor(n/2) */
    seq_t add   = (n & 1) ? (seq_t)10 : (seq_t)6;
    return base + add;
}

/*
 * A117797  Decagonal numbers divisible by 10.
 * https://oeis.org/A117797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (10*floor((n-1)/2) + ((n-1) mod 2)*2) *
 *            (4*(10*floor((n-1)/2)+((n-1) mod 2)*2) - 3)
 * Range:     u8 n <= 2, u16 n <= 26, u32 n <= 6554, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decagonal numbers divisible by 10, with a(1)=0. */
seq_t a(idx_t n)
{
    idx_t j = n - 1;
    idx_t q = j >> 1;          /* floor((n-1)/2) */
    idx_t r = j & 1;           /* (n-1) mod 2 */
    idx_t m = 10 * q + (r ? 2 : 0);
    seq_t M = (seq_t)m;
    return M * (((seq_t)4 * M) - (seq_t)3);
}

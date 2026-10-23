/*
 * A168437  a(n) = 3 + 10*floor(n/2).
 * https://oeis.org/A168437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 + 10*floor(n/2)
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; behaviour for n = 0 is irrelevant. */
    seq_t half = (seq_t)(n / 2);          /* floor(n/2) */
    return (seq_t)3 + (seq_t)10 * half;   /* 3 + 10 * floor(n/2) */
}

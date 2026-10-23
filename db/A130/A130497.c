/*
 * A130497  Repetition of odd numbers five times.
 * https://oeis.org/A130497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*floor(n/5)+1
 * Range:     u8 n <= 290, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t q = (seq_t)n / 5;
    return 2 * q + 1;
}

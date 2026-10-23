/*
 * A009056  Numbers >= 3.
 * https://oeis.org/A009056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + 2
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(1) = 3, a(2) = 4, a(3) = 5, ... */
seq_t a(idx_t n)
{
    /* Convert the uint64_t index to seq_t and add 2. */
    return (seq_t)n + (seq_t)2;
}

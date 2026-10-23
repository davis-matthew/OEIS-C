/*
 * A045534  Number of squarefree self-avoiding walks in 2 dimensions.
 * https://oeis.org/A045534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = 1,4,8,16,16,16,16,16 for 0 ≤ n ≤ 7; a(n) = 0 for n ≥ 8
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t vals[8] = {
        (seq_t)1,
        (seq_t)4,
        (seq_t)8,
        (seq_t)16,
        (seq_t)16,
        (seq_t)16,
        (seq_t)16,
        (seq_t)16
    };
    return (n < 8) ? vals[n] : (seq_t)0;
}

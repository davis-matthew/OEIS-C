/*
 * A083570  A de Bruijn sequence of length 9 over {0, 1, 2}, repeated.
 * https://oeis.org/A083570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = period[n mod 9] where period = {0,0,1,1,2,2,1,0,2}
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[9] = {
        (seq_t)0, (seq_t)0, (seq_t)1, (seq_t)1, (seq_t)2,
        (seq_t)2, (seq_t)1, (seq_t)0, (seq_t)2
    };
    return period[n % 9];
}

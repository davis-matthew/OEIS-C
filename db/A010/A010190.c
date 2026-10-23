/*
 * A010190  Continued fraction for sqrt(129).
 * https://oeis.org/A010190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 11; a(n) = [2,1,3,1,6,1,3,1,2,22][(n-1) mod 10] for n ≥ 1
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)11;
    static const seq_t pattern[10] = {
        (seq_t)2, (seq_t)1, (seq_t)3, (seq_t)1,
        (seq_t)6, (seq_t)1, (seq_t)3, (seq_t)1,
        (seq_t)2, (seq_t)22
    };
    return pattern[(n - 1) % 10];
}

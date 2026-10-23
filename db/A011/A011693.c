/*
 * A011693  A binary m-sequence: expansion of reciprocal of x^8+x^5+x^4+x^3+1.
 * https://oeis.org/A011693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-17) with period 17; the sequence repeats the 17
 *            binary pattern 0,0,0,0,0,0,0,1,0,0,1,1,1,1,0,0,1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* A minimal table of the 17-periodic pattern. */
    static const seq_t pat[17] = {
        (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0,
        (seq_t)0, (seq_t)0, (seq_t)1, (seq_t)0, (seq_t)0,
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)0,
        (seq_t)0, (seq_t)1
    };
    return pat[n % 17];
}

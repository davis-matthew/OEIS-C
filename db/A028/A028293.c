/*
 * A028293  Period 7.
 * https://oeis.org/A028293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Period 7: a(n) equals the n-th element of {1,2,1,3,1,2,1} modulo 7. */
seq_t a(idx_t n)
{
    static const seq_t cycle[7] = {
        (seq_t)1, (seq_t)2, (seq_t)1, (seq_t)3,
        (seq_t)1, (seq_t)2, (seq_t)1
    };
    return cycle[n % 7u];
}

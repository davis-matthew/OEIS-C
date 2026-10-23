/*
 * A157955  a(n) = 200*n - 1.
 * https://oeis.org/A157955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 200*n - 1
 * Range:     u8 n <= 1, u16 n <= 327, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. If n == 0 this function
       simply returns 0 (the behaviour for n < 1 is undefined in the
       OEIS entry). */
    if (n == 0)
        return (seq_t)0;
    return (seq_t)200 * (seq_t)n - (seq_t)1;
}

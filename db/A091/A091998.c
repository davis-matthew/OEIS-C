/*
 * A091998  Numbers that are congruent to {1, 11} mod 12.
 * https://oeis.org/A091998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n + 2*(-1)^n - 3 (a(1)=1)
 * Range:     u8 n <= 43, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numbers congruent to 1 or 11 modulo 12. */
seq_t a(idx_t n)
{
    /* Convert n to seq_t for arithmetic, but keep all ops within seq_t */
    seq_t n_seq = (seq_t)n;
    /* Base value for odd n: 6*n - 5 */
    seq_t val = 6*n_seq - (seq_t)5;
    /* For even n add 4 to get 6*n - 1 */
    if ((n & (idx_t)1) == 0)
        val += (seq_t)4;
    return val;
}

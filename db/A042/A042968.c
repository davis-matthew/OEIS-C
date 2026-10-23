/*
 * A042968  Numbers not divisible by 4.
 * https://oeis.org/A042968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((4*n-1)/3) (numbers not divisible by 4)
 * Range:     u8 n <= 192, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(1)=1, a(2)=2, a(3)=3, a(4)=5, a(5)=6, ... */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For safety handle n==0. */
    if (n == 0)
        return 0;
    /* Cast the index to seq_t first, then perform integer arithmetic. */
    return ((seq_t)n * 4 - 1) / 3;
}

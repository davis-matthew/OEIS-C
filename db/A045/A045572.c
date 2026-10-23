/*
 * A045572  Numbers that are odd but not divisible by 5.
 * https://oeis.org/A045572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + 2*floor((n+1)/4) - 1 (OEIS A045572)
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the n-th positive odd integer that is not divisible by 5.  */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 we return 0. */
    if (n == 0)
        return (seq_t)0;

    seq_t nn  = (seq_t)n;                    /* promote the index to seq_t */
    seq_t tmp = (nn + (seq_t)1) / (seq_t)4;  /* floor((n+1)/4)   */
    seq_t val = (seq_t)2 * nn + (seq_t)2 * tmp; /* 2*n + 2*floor((n+1)/4) */
    return val - (seq_t)1;                   /* subtract 1 to match OEIS formula */
}

/*
 * A047361  Numbers that are congruent to {0, 1, 2, 3} mod 7.
 * https://oeis.org/A047361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*floor((n-1)/4) + ((n-1) mod 4) (offset 1)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If called with n==0 the behaviour is undefined; return 0 for safety */
    if (n == 0)
        return 0;

    idx_t m = n - 1;                /* m = n-1 (non-negative) */
    seq_t mseq = (seq_t)m;          /* convert to seq_t domain */
    seq_t q = mseq / 4;             /* floor((n-1)/4) */
    seq_t r = mseq % 4;             /* (n-1) mod 4 */
    return q * 7 + r;               /* a(n) */
}

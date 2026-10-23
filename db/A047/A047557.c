/*
 * A047557  Numbers that are congruent to {0, 3, 6, 7} mod 8.
 * https://oeis.org/A047557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n-1-sin(Pi*n/2) (valid for n ≥ 1)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is at least 1 – behaviour for n == 0 is undefined in OEIS */
    seq_t n_seq = (seq_t)n;       /* promote index to seq_t */
    seq_t two_n = n_seq + n_seq;  /* 2*n */

    /* n % 4 determines the adjustment */
    switch (n % 4)
    {
        case 1:  /* sin = 1 => a = 2*n - 2 */
            return two_n - (seq_t)2;
        case 2:  /* sin = 0 => a = 2*n - 1 */
            return two_n - (seq_t)1;
        case 3:  /* sin = -1 => a = 2*n     */
            return two_n;
        default: /* n % 4 == 0, sin = 0 => a = 2*n - 1 */
            return two_n - (seq_t)1;
    }
}

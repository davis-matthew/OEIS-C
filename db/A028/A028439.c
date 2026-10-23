/*
 * A028439  Golc sequence in base 9. Left to right concatenation of
 *          n,int(log_9(n)),int(log_9(int(log_9(n)))),... in base9.
 * https://oeis.org/A028439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9*n + floor(log_9(n))
 * Range:     u8 n <= 28, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t t = n;
    seq_t exp = 0;
    while (t >= 9)
    {
        t /= 9;
        exp++;
    }
    return (seq_t)n * (seq_t)9 + exp;
}

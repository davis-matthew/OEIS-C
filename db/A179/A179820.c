/*
 * A179820  a(n) = n-th triangular number mod (n+2).
 * https://oeis.org/A179820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0; for n>0: a(n)=1 if n odd; otherwise a(n)=n/2+2
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n & 1U)
        return 1;
    return (seq_t)(n / 2U) + 2;
}

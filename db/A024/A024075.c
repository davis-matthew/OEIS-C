/*
 * A024075  a(n) = 7^n-1.
 * https://oeis.org/A024075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n-1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(n+1)=7*a(n)+6 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t i = 0; i < n; ++i)
    {
        res = res * 7 + 6;
    }
    return res;
}

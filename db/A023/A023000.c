/*
 * A023000  a(n) = (7^n - 1)/6.
 * https://oeis.org/A023000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(n)=7*a(n-1)+1 -- equivalent to a(n)=(7^n-1)/6 */
seq_t a(idx_t n)
{
    seq_t res = 0;                 /* a(0) */
    for (idx_t i = 0; i < n; ++i)  /* compute a(1)…a(n) */
        res = res * 7 + 1;
    return res;
}

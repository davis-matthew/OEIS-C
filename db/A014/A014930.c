/*
 * A014930  a(1)=1, a(n) = n*15^(n-1) + a(n-1).
 * https://oeis.org/A014930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n)=30*a(n-1)-225*a(n-2)+1; a(1)=1; a(2)=31; */
seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* undefined for n<1 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)31;

    seq_t a1 = (seq_t)1;          /* a(1) */
    seq_t a2 = (seq_t)31;         /* a(2) */
    seq_t ak;

    for (idx_t i = 3; i <= n; ++i) {
        ak = (seq_t)30 * a2 - (seq_t)225 * a1 + (seq_t)1;
        a1 = a2;
        a2 = ak;
    }
    return a2;
}

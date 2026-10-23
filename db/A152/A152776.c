/*
 * A152776  Numbers such that every run length in base 2 is 3.
 * https://oeis.org/A152776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 8*a(n-1) + a(n-2) - 8*a(n-3); Initial values: a(1)=7,
 *            a(2)=56, a(3)=455
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* behaviour below the offset is irrelevant */
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)56;
    if (n == 3) return (seq_t)455;

    /* Recurrence variables: a1=a(i-3), a2=a(i-2), a3=a(i-1) */
    seq_t a1 = (seq_t)7;
    seq_t a2 = (seq_t)56;
    seq_t a3 = (seq_t)455;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t cur = 8 * a3 + a2 - 8 * a1;  /* a(i) = 8*a(i-1) + a(i-2) - 8*a(i-3) */
        a1 = a2;
        a2 = a3;
        a3 = cur;
    }

    return a3;
}

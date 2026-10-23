/*
 * A027024  a(n) = T(n,n+2), T given by A027023.
 * https://oeis.org/A027024
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+8 for n>4 with base values a(2)=1,
 *            a(3)=5, a(4)=13
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 72, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;  /* outside offset, value irrelevant */

    if (n == 2)
        return (seq_t)1;
    if (n == 3)
        return (seq_t)5;
    if (n == 4)
        return (seq_t)13;

    seq_t f1 = (seq_t)1;   /* a(2) */
    seq_t f2 = (seq_t)5;   /* a(3) */
    seq_t f3 = (seq_t)13;  /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = f1 + f2 + f3 + (seq_t)8;
        f1 = f2;
        f2 = f3;
        f3 = next;
    }
    return f3;
}

/*
 * A153600  a(n) = ((9 + sqrt(3))^n - (9 - sqrt(3))^n)/(2*sqrt(3)).
 * https://oeis.org/A153600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((9 + sqrt(3))^n - (9 - sqrt(3))^n)/(2*sqrt(3))
 *            Recurrence: a(1)=1, a(2)=18, a(n)=18*a(n-1)-78*a(n-2) for n>2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)18;

    seq_t prev = (seq_t)1;   /* a(1) */
    seq_t curr = (seq_t)18;  /* a(2) */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t next = curr * (seq_t)18 - prev * (seq_t)78;
        prev = curr;
        curr = next;
    }
    return curr;
}

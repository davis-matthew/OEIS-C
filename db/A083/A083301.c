/*
 * A083301  a(n) = (4*8^n + (-2)^n)/5.
 * https://oeis.org/A083301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4*8^n + (-2)^n)/5 Recurrence: a(0)=1, a(1)=6,
 *            a(n)=6*a(n-1)+16*a(n-2) for n>=2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    if (n == (idx_t)1)
        return (seq_t)6;

    seq_t prev = (seq_t)1;     /* a(0) */
    seq_t curr = (seq_t)6;     /* a(1) */

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        seq_t next = (curr * (seq_t)6) + (prev * (seq_t)16);
        prev = curr;
        curr = next;
    }

    return curr;
}

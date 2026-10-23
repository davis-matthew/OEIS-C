/*
 * A147961  a(n) = ((6+sqrt(3))^n + (6-sqrt(3))^n)/2.
 * https://oeis.org/A147961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((6+sqrt(3))^n + (6-sqrt(3))^n)/2. Recurrence: a(0)=1,
 *            a(1)=6, a(n)=12*a(n-1)-33*a(n-2) for n>1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    seq_t prev = (seq_t)1;  /* a(0) */
    seq_t cur  = (seq_t)6;  /* a(1) */
    seq_t next;
    idx_t i;

    for (i = 2; i <= n; ++i) {
        next = (seq_t)12 * cur - (seq_t)33 * prev;
        prev = cur;
        cur  = next;
    }
    return cur;
}

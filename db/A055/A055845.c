/*
 * A055845  a(n) = 4*a(n-1) - a(n-2) with a(0)=1, a(1)=8.
 * https://oeis.org/A055845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) - a(n-2),  a(0) = 1,  a(1) = 8 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    seq_t prev = (seq_t)1;   /* a(0) */
    seq_t curr = (seq_t)8;   /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = (seq_t)4 * curr - prev;
        prev = curr;
        curr = next;
    }
    return curr;              /* a(n) */
}

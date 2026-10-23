/*
 * A165311  a(0)=1, a(1)=4, a(n)=9*a(n-1)-16*a(n-2) for n>1.
 * https://oeis.org/A165311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 4, a(n) = 9*a(n-1) - 16*a(n-2) for n > 1 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)4;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t a2 = (seq_t)9 * a1 - (seq_t)16 * a0;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}

/*
 * A012866  a(n+3) = 5*a(n+2)-4*a(n+1)+a(n).
 * https://oeis.org/A012866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+3) = 5*a(n+2)-4*a(n+1)+a(n)
   Base values: a(0) = 0, a(1) = 1, a(2) = 2
*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)2;
    seq_t a3;

    for (idx_t i = 3; i <= n; ++i) {
        a3 = ((seq_t)5) * a2 - ((seq_t)4) * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}

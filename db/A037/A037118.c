/*
 * A037118  Trajectory of 3 under map n->41n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A037118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 38, u64 n <= 57, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=3; a(n)= a(n−1) odd ? 41*a(n−1)+1 : a(n−1)/2 */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)3;
    if (n == 0) return x;
    for (idx_t i = 1; i <= n; ++i) {
        if (x & 1)        /* odd */
            x = x * 41 + 1;
        else              /* even */
            x = x >> 1;   /* x / 2 */
    }
    return x;
}

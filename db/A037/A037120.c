/*
 * A037120  Trajectory of 3 under map n->45n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A037120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 46, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 3; for n > 0, a(n) = 45*a(n-1)+1 if a(n-1) is odd, else a(n-1)/2. */
seq_t a(idx_t n)
{
    seq_t v = 3;
    for (idx_t i = 0; i < n; ++i) {
        if (v & 1) {
            v = v * (seq_t)45 + (seq_t)1;
        } else {
            v = v >> 1;
        }
    }
    return v;
}

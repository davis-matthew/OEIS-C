/*
 * A218989  Power ceiling sequence of 2+sqrt(8).
 * https://oeis.org/A218989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 4*a(n-3) (n ≥ 3), with initial values a(0)=5,
 *            a(1)=25, a(2)=121
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)25;
    if (n == 2) return (seq_t)121;

    /* store a(n-3), a(n-2), a(n-1) */
    seq_t v[3];
    v[0] = (seq_t)5;      /* a(0) */
    v[1] = (seq_t)25;     /* a(1) */
    v[2] = (seq_t)121;    /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = 5 * v[2] - 4 * v[0];
        v[0] = v[1];
        v[1] = v[2];
        v[2] = next;
    }

    return v[2];
}

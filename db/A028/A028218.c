/*
 * A028218  Expansion of 1/((1-6x)(1-9x)(1-11x)(1-12x)).
 * https://oeis.org/A028218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 38*a(n-1) - 531*a(n-2) + 3222*a(n-3) - 7128*a(n-4)
 *            Initial conditions: a(0)=1, a(1)=38, a(2)=913, a(3)=17738
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)38;
    if (n == 2) return (seq_t)913;
    if (n == 3) return (seq_t)17738;

    /* Use a sliding window of the four most recent terms */
    seq_t a0 = (seq_t)1;             /* a(n-4) */
    seq_t a1 = (seq_t)38;            /* a(n-3) */
    seq_t a2 = (seq_t)913;           /* a(n-2) */
    seq_t a3 = (seq_t)17738;         /* a(n-1) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        next = (seq_t)(38 * a3 - 531 * a2 + 3222 * a1 - 7128 * a0);
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}

/*
 * A189744  a(1)=4, a(2)=5, a(n) = 4*a(n-1) + 5*a(n-2).
 * https://oeis.org/A189744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 5*a(n-2), a(1)=4, a(2)=5
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset not defined, return 0 */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)5;

    seq_t a1 = 4;   /* a(1) */
    seq_t a2 = 5;   /* a(2) */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        next = 4 * a2 + 5 * a1;  /* compute a(i) */
        a1   = a2;              /* shift previous terms */
        a2   = next;
    }

    return a2;  /* a(n) */
}

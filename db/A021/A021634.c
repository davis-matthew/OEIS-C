/*
 * A021634  Expansion of 1/((1-x)(1-3x)(1-8x)(1-9x)).
 * https://oeis.org/A021634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (35*9^(n+3) - 48*8^(n+3) + 28*3^(n+3) - 15)/1680
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the rational generating function:    */
/* a(n) = 21*a(n-1) - 143*a(n-2) + 339*a(n-3) - 216*a(n-4)   */
/* Initial values: a(0)=1, a(1)=21, a(2)=298, a(3)=3594 */

/* No dynamic memory allocation or external dependencies are used. */
seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)21;
    if (n == 2u) return (seq_t)298;
    if (n == 3u) return (seq_t)3594;

    /* Last four computed terms of the recurrence. */
    seq_t a0 = (seq_t)1;        /* a(n-4) */
    seq_t a1 = (seq_t)21;       /* a(n-3) */
    seq_t a2 = (seq_t)298;      /* a(n-2) */
    seq_t a3 = (seq_t)3594;     /* a(n-1) */

    for (idx_t i = 4u; i <= n; ++i) {
        seq_t next = a3 * 21 - a2 * 143 + a1 * 339 - a0 * 216;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}

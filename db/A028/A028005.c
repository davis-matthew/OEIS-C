/*
 * A028005  Expansion of 1/((1-2x)(1-7x)(1-8x)(1-9x)).
 * https://oeis.org/A028005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 26*a(n-1) - 239*a(n-2) + 886*a(n-3) - 1008*a(n-4), with
 *            a(0)=1, a(1)=26, a(2)=437, a(3)=6034. The sequence is the
 *            expansion of 1/((1-2x)(1-7x)(1-8x)(1-9x))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)26;
    if (n == 2) return (seq_t)437;
    if (n == 3) return (seq_t)6034;

    seq_t a0 = (seq_t)1,    /* a(n-4)   */
           a1 = (seq_t)26,   /* a(n-3)   */
           a2 = (seq_t)437,  /* a(n-2)   */
           a3 = (seq_t)6034; /* a(n-1)   */

    for (idx_t i = 4; i <= n; ++i) {
        /* compute a(i) without intermediate negative values.
           All terms are unsigned, so we order the operations so
           that the subtractions are safe. */
        seq_t term1 = (seq_t)26 * a3 + (seq_t)886 * a1;
        seq_t term2 = (seq_t)239 * a2 + (seq_t)1008 * a0;
        seq_t next  = term1 - term2;

        /* shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}

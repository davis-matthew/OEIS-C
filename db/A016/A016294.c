/*
 * A016294  Expansion of 1/((1-2x)(1-4x)(1-12x)).
 * https://oeis.org/A016294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*a(n-1) - 80*a(n-2) + 96*a(n-3) (n ≥ 3), with initial
 *            values a(0) = 1, a(1) = 18, a(2) = 244
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)18;
    if (n == 2) return (seq_t)244;

    seq_t a0 = (seq_t)1;     /* a(0)   */
    seq_t a1 = (seq_t)18;    /* a(1)   */
    seq_t an = (seq_t)244;   /* a(2)   */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = (seq_t)18 * an - (seq_t)80 * a1 + (seq_t)96 * a0;
        a0 = a1;
        a1 = an;
        an = next;
    }
    return an;
}

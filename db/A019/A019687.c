/*
 * A019687  Expansion of 1/((1-4x)(1-9x)(1-11x)).
 * https://oeis.org/A019687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*a(n-1) - 179*a(n-2) + 396*a(n-3); initial values:
 *            a(0)=1, a(1)=24, a(2)=397
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)24;
    if (n == 2) return (seq_t)397;

    seq_t a0 = (seq_t)1;   /* a(n-3)   */
    seq_t a1 = (seq_t)24;  /* a(n-2)   */
    seq_t a2 = (seq_t)397; /* a(n-1)   */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = 24 * a2 - 179 * a1 + 396 * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}

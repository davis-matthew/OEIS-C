/*
 * A016298  Expansion of 1/((1-2x)(1-5x)(1-9x)).
 * https://oeis.org/A016298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1) - 73*a(n-2) + 90*a(n-3); a(0)=1, a(1)=16,
 *            a(2)=183
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)183;

    seq_t a0 = (seq_t)1;   /* a(n-3)   */
    seq_t a1 = (seq_t)16;  /* a(n-2)   */
    seq_t a2 = (seq_t)183; /* a(n-1)   */
    seq_t next;
    idx_t i;

    for (i = 3; i <= n; ++i) {
        next = (seq_t)(16) * a2 - (seq_t)(73) * a1 + (seq_t)(90) * a0;
        a0 = a1; /* shift forward: a(n-2) becomes a(n-3) */
        a1 = a2; /* shift forward: a(n-1) becomes a(n-2) */
        a2 = next; /* new a(n) becomes a(n-1) for next iteration */
    }
    return a2;
}

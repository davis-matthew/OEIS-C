/*
 * A135530  a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3), with a(0)=2, a(1)=1.
 * https://oeis.org/A135530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3); a(0)=2, a(1)=1, a(2)=4
 * Range:     u8 n <= 13, u16 n <= 29, u32 n <= 61, u64 n <= 125,
 *            u128 n <= 253, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    seq_t a0 = (seq_t)2;   /* a(n-3) for start of loop (i=3) */
    seq_t a1 = (seq_t)1;   /* a(n-2) for start of loop */
    seq_t a2 = (seq_t)4;   /* a(n-1) for start of loop */
    seq_t ai = (seq_t)0;   /* placeholder for a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        ai = a2 + (seq_t)2 * a1 - (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return ai;
}

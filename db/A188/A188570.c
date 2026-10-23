/*
 * A188570  a(n) = coefficient of the term that is independent of sqrt(2) and
 *          sqrt(3) in the expansion of (1 + sqrt(2) + sqrt(3))^n.
 * https://oeis.org/A188570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=6, a(3)=16,
 *            a(n)=4*a(n-1)+4*a(n-2)-16*a(n-3)+8*a(n-4) (OEIS A188570)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)16;

    seq_t a0 = (seq_t)1;   /* a(n-4)   */
    seq_t a1 = (seq_t)1;   /* a(n-3)   */
    seq_t a2 = (seq_t)6;   /* a(n-2)   */
    seq_t a3 = (seq_t)16;  /* a(n-1)   */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        an = 4 * a3 + 4 * a2 + 8 * a0; /* compute sum first to avoid unsigned underflow */
        an -= 16 * a1;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}

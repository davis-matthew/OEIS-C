/*
 * A054147  a(n) = T(2n,n), array T as in A054144.
 * https://oeis.org/A054147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 20*a(n-2) + 16*a(n-3) - 4*a(n-4) Initial
 *            values: a(0)=0, a(1)=3, a(2)=21, a(3)=108
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)21;
    if (n == 3) return (seq_t)108;

    seq_t p4 = (seq_t)0;   /* a(n‑4) */
    seq_t p3 = (seq_t)3;   /* a(n‑3) */
    seq_t p2 = (seq_t)21;  /* a(n‑2) */
    seq_t p1 = (seq_t)108; /* a(n‑1) */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i) {
        /* compute cur = 8*p1 - 20*p2 + 16*p3 - 4*p4
           grouping positive terms first avoids unsigned underflow   */
        seq_t pos = (seq_t)8 * p1 + (seq_t)16 * p3;
        seq_t neg = (seq_t)20 * p2 + (seq_t)4 * p4;
        cur   = pos - neg;
        p4    = p3;
        p3    = p2;
        p2    = p1;
        p1    = cur;
    }
    return p1;
}

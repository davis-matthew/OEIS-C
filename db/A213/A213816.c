/*
 * A213816  Tribonacci sequences A000073 and A001590 interleaved.
 * https://oeis.org/A213816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=a(2)=a(3)=1; for n > 1: a(2n) = a(2n-1) + a(2n-3) a(2n+1) =
 *            a(2n-1) + a(2n-2)
 * Range:     u8 n <= 20, u16 n <= 38, u32 n <= 75, u64 n <= 147,
 *            u128 n <= 293, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3)
        return 1;  /* a(1)=a(2)=a(3)=1 */

    /* a0 = a(i-3), a1 = a(i-2), a2 = a(i-1) initially for i = 4 */
    seq_t a0 = 1; /* a(1) */
    seq_t a1 = 1; /* a(2) */
    seq_t a2 = 1; /* a(3) */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i)
    {
        if ((i & 1) == 0)          /* even index: a(2n) = a(2n-1) + a(2n-3) */
            cur = a2 + a0;
        else                       /* odd index:  a(2n+1) = a(2n-1) + a(2n-2) */
            cur = a1 + a0;

        if (i == n)
            return cur;

        /* shift window: a0 ← a1, a1 ← a2, a2 ← cur */
        a0 = a1;
        a1 = a2;
        a2 = cur;
    }

    /* unreachable, but satisfies the compiler */
    return 0;
}

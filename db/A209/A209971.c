/*
 * A209971  a(n) = A000129(n) + n.
 * https://oeis.org/A209971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Pell(n) + n where Pell numbers are P(0)=0, P(1)=1, and
 *            P(n) = 2*P(n-1) + P(n-2) for n ≥ 2. This implements a(n) for the
 *            OEIS sequence A209971 with offset 0
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the first term directly. */
    if (n == 0u) return 0;

    /* Pell numbers: P(0)=0, P(1)=1. */
    seq_t p0 = 0;      /* P(n-2) */
    seq_t p1 = 1;      /* P(n-1) */

    if (n == 1u) return (seq_t)1 + (seq_t)1;  /* P(1)+1 = 2 */

    seq_t p;           /* P(n) */
    for (idx_t i = 2; i <= n; ++i) {
        /* P(i) = 2*P(i-1) + P(i-2) */
        p = 2 * p1 + p0;
        p0 = p1;
        p1 = p;
    }

    /* a(n) = Pell(n) + n */
    return p1 + (seq_t)n;
}

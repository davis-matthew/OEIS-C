/*
 * A134465  Row sums of triangle A134464.
 * https://oeis.org/A134465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n−1) – 6*a(n−2) + 4*a(n−3) – a(n−4) Initial values:
 *            a(1)=1, a(2)=6, a(3)=16, a(4)=32
 * Range:     u8 n <= 9, u16 n <= 70, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* offset not defined below 1 */

    /* return the known first four terms directly */
    if (n == 1) return 1;
    if (n == 2) return 6;
    if (n == 3) return 16;
    if (n == 4) return 32;

    /* a1 .. a4 hold a(n‑4), a(n‑3), a(n‑2), a(n‑1) respectively */
    seq_t a1 = 1;   /* a(1) */
    seq_t a2 = 6;   /* a(2) */
    seq_t a3 = 16;  /* a(3) */
    seq_t a4 = 32;  /* a(4) */
    seq_t cur;

    for (idx_t i = 5; i <= n; ++i) {
        cur = (seq_t)4 * a4;          /* 4*a(n‑1)   */
        cur -= (seq_t)6 * a3;          /* -6*a(n‑2)  */
        cur += (seq_t)4 * a2;          /* +4*a(n‑3)  */
        cur -= a1;                    /* -a(n‑4)    */

        /* slide the window for the next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = cur;
    }

    return a4;
}

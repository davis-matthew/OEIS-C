/*
 * A216796  Number of permutations of an n X 4 array with each element moving
 *          exactly one horizontally or vertically and without 2-loops.
 * https://oeis.org/A216796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) +10*a(n-2) -28*a(n-3) -7*a(n-4) +30*a(n-5)
 *            -8*a(n-6) (n ≥ 7) Initial values: a(1)=0, a(2)=6, a(3)=8,
 *            a(4)=88, a(5)=292, a(6)=1774
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for offset 1, but safe */

    /* Fast return for the first six terms */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)8;
    if (n == 4) return (seq_t)88;
    if (n == 5) return (seq_t)292;
    if (n == 6) return (seq_t)1774;

    /* Start with the six preceding values a(n-6) … a(n-1) */
    seq_t a1 = (seq_t)0;     /* a(n-6) */
    seq_t a2 = (seq_t)6;     /* a(n-5) */
    seq_t a3 = (seq_t)8;     /* a(n-4) */
    seq_t a4 = (seq_t)88;    /* a(n-3) */
    seq_t a5 = (seq_t)292;   /* a(n-2) */
    seq_t a6 = (seq_t)1774;  /* a(n-1) */

    /* Iterate until the desired index is reached */
    for (idx_t i = 7; i <= n; ++i) {
        seq_t next = 4 * a6 + 10 * a5 - 28 * a4 - 7 * a3 + 30 * a2 - 8 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = next;
    }
    return a6;
}

/*
 * A183118  Magnetic Tower of Hanoi, total number of moves, optimally solving
 *          the [NEUTRAL ; NEUTRAL ; NEUTRAL] pre-colored puzzle.
 * https://oeis.org/A183118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=4, a(3)=11, a(4)=30, a(5)=83. For n ≥ 6:
 *            a(n) = 4*a(n-1) - 2*a(n-2) - 2*a(n-3) - 5*a(n-4) + 6*a(n-5)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)11;
    if (n == 4) return (seq_t)30;
    if (n == 5) return (seq_t)83;

    /* Initialize the five preceding terms */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)4;   /* a(2) */
    seq_t a3 = (seq_t)11;  /* a(3) */
    seq_t a4 = (seq_t)30;  /* a(4) */
    seq_t a5 = (seq_t)83;  /* a(5) */

    seq_t next;
    for (idx_t k = 6; k <= n; ++k) {
        /* a(k) = 4*a5 - 2*a4 - 2*a3 - 5*a2 + 6*a1 */
        next = 4 * a5
             - 2 * a4
             - 2 * a3
             - 5 * a2
             + 6 * a1;

        /* shift terms for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }
    return a5;
}

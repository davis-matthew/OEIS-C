/*
 * A025995  Expansion of 1/((1-2x)(1-5x)(1-7x)(1-11x)).
 * https://oeis.org/A025995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -8*2^n/135 + 125*5^n/36 - 343*7^n/40 + 1331*11^n/216
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values obtained directly from the generating function */
    seq_t a0 = 1;   /* a(0) */
    seq_t a1 = 25;  /* a(1) */
    seq_t a2 = 412; /* a(2) */
    seq_t a3 = 5694;/* a(3) */

    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;

    /* The recurrence follows from the expansion
       (1-2x)(1-5x)(1-7x)(1-11x) = 1-25x+213x²-719x³+770x⁴
       Hence
       a(n) = 25a(n-1) - 213a(n-2) + 719a(n-3) - 770a(n-4)
    */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t t1 = (seq_t)25 * a3;
        seq_t t2 = (seq_t)213 * a2;
        seq_t t3 = (seq_t)719 * a1;
        seq_t t4 = (seq_t)770 * a0;

        seq_t ai = t1 - t2 + t3 - t4;

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}

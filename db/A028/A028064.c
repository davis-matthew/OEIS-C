/*
 * A028064  Expansion of 1/((1-3x)(1-5x)(1-8x)(1-9x)).
 * https://oeis.org/A028064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A028064
 * 
 * Formula: a(n) = (5*9^(n+3) - 8^(n+4) + 5^(n+4) - 2*3^(n+3)) / 120
 * Recurrence: a(n) = 25*a(n-1) - 223*a(n-2) + 831*a(n-3) - 1080*a(n-4)
 * Initial terms: a(0)=1, a(1)=25, a(2)=402, a(3)=5306.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)25;
    if (n == 2) return (seq_t)402;
    if (n == 3) return (seq_t)5306;

    seq_t a0 = (seq_t)1;     /* a(n-4) */
    seq_t a1 = (seq_t)25;    /* a(n-3) */
    seq_t a2 = (seq_t)402;   /* a(n-2) */
    seq_t a3 = (seq_t)5306;  /* a(n-1) */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)25 * a3
            - (seq_t)223 * a2
            + (seq_t)831 * a1
            - (seq_t)1080 * a0;

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}

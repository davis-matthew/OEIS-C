/*
 * A158920  Binomial transform of A008805 (triangular numbers with repeats).
 * https://oeis.org/A158920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 12*a(n-2) + 8*a(n-3) for n>5 a(1)=1, a(2)=2,
 *            a(3)=6, a(4)=16, a(5)=41
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 58, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* outside the offset, undefined behaviour otherwise */
    if (n <= 5) {
        static const seq_t base[5] = {
            (seq_t)1,
            (seq_t)2,
            (seq_t)6,
            (seq_t)16,
            (seq_t)41
        };
        return base[n-1];
    }

    /* Initialise for n = 3,4,5 */
    seq_t a1 = (seq_t)6;  /* a(3) */
    seq_t a2 = (seq_t)16; /* a(4) */
    seq_t a3 = (seq_t)41; /* a(5) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = (seq_t)6 * a3 - (seq_t)12 * a2 + (seq_t)8 * a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}

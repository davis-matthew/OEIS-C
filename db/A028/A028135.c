/*
 * A028135  Expansion of 1/((1-4x)(1-6x)(1-8x)(1-9x)).
 * https://oeis.org/A028135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 27*a(n-1) - 266*a(n-2) + 1128*a(n-3) - 1728*a(n-4),
   with initial values a(0)=1, a(1)=27, a(2)=463, a(3)=6447. */
seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 27;
    if (n == 2) return 463;
    if (n == 3) return 6447;

    seq_t a0 = 1;    /* a(n-4)  */
    seq_t a1 = 27;   /* a(n-3)  */
    seq_t a2 = 463;  /* a(n-2)  */
    seq_t a3 = 6447; /* a(n-1)  */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        an = 27 * a3 - 266 * a2 + 1128 * a1 - 1728 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}

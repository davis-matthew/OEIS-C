/*
 * A025031  Expansion of 1/((1-x)*(1-9*x)*(1-10*x)*(1-12*x)).
 * https://oeis.org/A025031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A025031: Expansion of 1/((1-x)*(1-9*x)*(1-10*x)*(1-12*x))
   a(0)=1, a(1)=32, a(2)=675, a(3)=11830
   a(n) = 32*a(n-1) - 349*a(n-2) + 1398*a(n-3) - 1080*a(n-4)   for n ≥ 4 */
seq_t a(idx_t n)
{
    /* handle the seed values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)32;
    if (n == 2) return (seq_t)675;
    if (n == 3) return (seq_t)11830;

    /* initialise the last four values */
    seq_t a_n_4 = 1;      /* a(0)  */
    seq_t a_n_3 = 32;     /* a(1)  */
    seq_t a_n_2 = 675;    /* a(2)  */
    seq_t a_n_1 = 11830;  /* a(3)  */
    seq_t a_n;            /* placeholder for a(n) */

    /* iterate from 4 up to n, advancing the sliding window */
    for (idx_t i = 4; i <= n; ++i) {
        a_n = ((seq_t)32   * a_n_1)
            - ((seq_t)349  * a_n_2)
            + ((seq_t)1398 * a_n_3)
            - ((seq_t)1080 * a_n_4);

        /* shift the window for the next loop iteration */
        a_n_4 = a_n_3;
        a_n_3 = a_n_2;
        a_n_2 = a_n_1;
        a_n_1 = a_n;
    }

    return a_n;
}

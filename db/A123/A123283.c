/*
 * A123283  X-values of solutions to the equation 3(X-Y)^4 - 8*X*Y = 0 with X
 *          >= Y.
 * https://oeis.org/A123283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 108*a(n-1) - 982*a(n-2) + 108*a(n-3) - a(n-4)
 * with a(0)=0, a(1)=12, a(2)=1000, a(3)=96228 */
seq_t a(idx_t n)
{
    /* handle the base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)1000;
    if (n == 3) return (seq_t)96228;

    /* iterate from 4 up to n computing the sequence term by term */
    seq_t a0 = (seq_t)0;      /* a(n-4) */
    seq_t a1 = (seq_t)12;     /* a(n-3) */
    seq_t a2 = (seq_t)1000;   /* a(n-2) */
    seq_t a3 = (seq_t)96228;  /* a(n-1) */
    seq_t an = a3;            /* will hold a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        an = ((seq_t)108) * a3
          - ((seq_t)982) * a2
          + ((seq_t)108) * a1
          - a0;

        /* shift the window forward */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return an;
}

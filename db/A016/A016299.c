/*
 * A016299  Expansion of 1/((1-2*x)*(1-5*x)*(1-10*x)).
 * https://oeis.org/A016299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1) - 80*a(n-2) + 100*a(n-3); Initial values:
 *            a(0)=1, a(1)=17, a(2)=209
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the three trivial cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)17;
    if (n == 2) return (seq_t)209;

    /* Recurrence variables */
    seq_t v0 = (seq_t)1;    /* a(0)  */
    seq_t v1 = (seq_t)17;   /* a(1)  */
    seq_t v2 = (seq_t)209;  /* a(2)  */
    seq_t v3 = v2;          /* will hold a(i) as we iterate */

    for (idx_t i = 3; i <= n; ++i) {
        v3 = (seq_t)17 * v2 - (seq_t)80 * v1 + (seq_t)100 * v0;
        /* shift the window: a(i-2), a(i-1), a(i) become the new v0,v1,v2 */
        v0 = v1;
        v1 = v2;
        v2 = v3;
    }
    return v3;
}

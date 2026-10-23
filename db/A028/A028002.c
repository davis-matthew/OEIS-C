/*
 * A028002  Expansion of 1/((1-2*x)*(1-6*x)*(1-10*x)*(1-11*x)).
 * https://oeis.org/A028002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 29*a(n-1) - 290*a(n-2) + 1132*a(n-3) - 1320*a(n-4) for n >= 4
   with a(0)=1, a(1)=29, a(2)=551, a(3)=8701
*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)29;
    if (n == 2) return (seq_t)551;
    if (n == 3) return (seq_t)8701;

    seq_t a0 = (seq_t)1,  /* a(n-4) */
           a1 = (seq_t)29, /* a(n-3) */
           a2 = (seq_t)551,/* a(n-2) */
           a3 = (seq_t)8701;/* a(n-1) */
    seq_t an;
    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)29 * a3
            - (seq_t)290 * a2
            + (seq_t)1132 * a1
            - (seq_t)1320 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}

/*
 * A017952  Expansion of 1/((1-3x)*(1-6x)*(1-10x)).
 * https://oeis.org/A017952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*10^(n+2) - 7*6^(n+2) + 4*3^(n+2))/84
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=19, a(2)=253 */
/*            a(n)=19*a(n-1) - 108*a(n-2) + 180*a(n-3)  for n>2 */
seq_t a(idx_t n)
{
    /* initial values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)19;
    if (n == 2) return (seq_t)253;

    seq_t a0 = (seq_t)1;           /* a(n-3) */
    seq_t a1 = (seq_t)19;          /* a(n-2) */
    seq_t a2 = (seq_t)253;         /* a(n-1) */
    seq_t an  = a2;

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)19 * a2 - (seq_t)108 * a1 + (seq_t)180 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}

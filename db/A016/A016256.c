/*
 * A016256  Expansion of 1/((1-x)*(1-8*x)*(1-9*x)).
 * https://oeis.org/A016256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=18, a(n)=17*a(n-1)-72*a(n-2)+1 (n>=2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)18;
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)18;  /* a(1) */
    seq_t an = 0;
    for (idx_t i = 2; i <= n; ++i) {
        an  = (seq_t)17 * a1;
        an -= (seq_t)72 * a0;
        an += (seq_t)1;
        a0 = a1;
        a1 = an;
    }
    return an;
}

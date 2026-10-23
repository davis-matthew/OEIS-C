/*
 * A049008  Greatest possible number of right angles that can occur as
 *          interior angles in a planar n-gon.
 * https://oeis.org/A049008
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor( (2*(n+2) – 1) / 3 ) for n ≥ 6, with a(3)=1,
 *            a(4)=4, a(5)=3. This matches OEIS A049008
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Undefined for n < 3 – return 0 (offset behavior). */
    if (n < 3) return (seq_t)0;

    /* Handle the few exceptional small values. */
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)4;
    if (n == 5) return (seq_t)3;

    /* For n ≥ 6 use the closed form.  Use seq_t arithmetic
       to avoid any loss of precision with arbitrary-precision builds. */
    seq_t v = (seq_t)(n + 2);   /* n+2 fits safely in seq_t */
    v = v * (seq_t)2;           /* 2*(n+2) */
    v = v - (seq_t)1;           /* 2*(n+2) – 1 */
    return v / (seq_t)3;        /* floor division */
}

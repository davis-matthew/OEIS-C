/*
 * A084231  Numbers k such that the root-mean-square value of 1, 2, ..., k,
 *          i.e., sqrt((1/k)*Sum_{j=1..k} j^2), is an integer.
 * https://oeis.org/A084231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 195*a(n-1) - 195*a(n-2) + a(n-3) for n ≥ 4 Initial
 *            values: a(1)=1, a(2)=337, a(3)=65521 (offset 1 as in OEIS)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle first three terms directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)337;
    if (n == 3) return (seq_t)65521;

    /* Iteratively compute using the recurrence */
    seq_t a1 = (seq_t)1;          /* a(n-3)  */
    seq_t a2 = (seq_t)337;        /* a(n-2)  */
    seq_t a3 = (seq_t)65521;      /* a(n-1)  */
    seq_t an;                     /* a(n)   */

    for (idx_t i = 4; i <= n; ++i) {
        an = a3 * (seq_t)195 - a2 * (seq_t)195 + a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}

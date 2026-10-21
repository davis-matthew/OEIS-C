/*
 * A215493  a(n) = 7*a(n-1) - 14*a(n-2) + 7*a(n-3) with a(0)=0, a(1)=1,
 *          a(2)=4.
 * https://oeis.org/A215493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 14*a(n-2) + 7*a(n-3). Initial values: a(0)=0,
 *            a(1)=1, a(2)=4
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the three base cases directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    /* Iteratively compute the recurrence up to n.
       Three variables hold the last three terms; they are of type seq_t to
       avoid any overflow in the intermediate multiplication. */
    seq_t a0 = (seq_t)0;   /* a(i-3)   */
    seq_t a1 = (seq_t)1;   /* a(i-2)   */
    seq_t a2 = (seq_t)4;   /* a(i-1)   */
    seq_t ai;              /* a(i)     */

    for (idx_t i = 3; i <= n; ++i) {
        ai = 7 * a2 - 14 * a1 + 7 * a0;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}

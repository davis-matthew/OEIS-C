/*
 * A024495  a(n) = C(n,2) + C(n,5) + ... + C(n, 3*floor(n/3)+2).
 * https://oeis.org/A024495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) (n ≥ 3) Initial values:
 *            a(0) = 0, a(1) = 0, a(2) = 1
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* Subsequent terms are computed using the linear recurrence. */
    seq_t a0 = (seq_t)0;   /* a(n-3)    */
    seq_t a1 = (seq_t)0;   /* a(n-2)    */
    seq_t a2 = (seq_t)1;   /* a(n-1)    */
    seq_t an = a2;         /* placeholder for a(n)   */

    for (idx_t i = 3; i <= n; ++i) {
        an = 3 * a2 - 3 * a1 + 2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }

    return an;
}

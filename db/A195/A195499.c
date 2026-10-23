/*
 * A195499  Denominators a(n) of Pythagorean approximations b(n)/a(n) to
 *          sqrt(3).
 * https://oeis.org/A195499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) - a(n-3) for n > 3; a(1)=3, a(2)=8,
 *            a(3)=33
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* outside the defined offset */
    if (n == 1) return 3;
    if (n == 2) return 8;
    if (n == 3) return 33;

    /* a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) */
    seq_t a1 = 3;   /* a(1)   */
    seq_t a2 = 8;   /* a(2)   */
    seq_t a3 = 33;  /* a(3)   */
    seq_t a_next;

    for (idx_t i = 4; i <= n; ++i) {
        a_next = 3 * a3 + 3 * a2 - a1;
        a1 = a2;
        a2 = a3;
        a3 = a_next;
    }
    return a3;
}

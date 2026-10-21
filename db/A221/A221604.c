/*
 * A221604  Number of n X 3 arrays of occupancy after each element moves to
 *          some horizontal or antidiagonal neighbor, without move-in move-out
 *          left turns.
 * https://oeis.org/A221604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 4*a(n-2) + 2*a(n-3) - 2*a(n-4) Initial values:
 *            a(1)=2, a(2)=10, a(3)=54, a(4)=287 (OEIS A221604, offset 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* small constant terms */
    const seq_t A1 = (seq_t)2;
    const seq_t A2 = (seq_t)10;
    const seq_t A3 = (seq_t)54;
    const seq_t A4 = (seq_t)287;

    if (n == 1) return A1;
    if (n == 2) return A2;
    if (n == 3) return A3;
    if (n == 4) return A4;

    /* iterate the linear recurrence */
    seq_t a1 = A1, a2 = A2, a3 = A3, a4 = A4, next;
    for (idx_t i = 5; i <= n; ++i) {
        next = 6 * a4 - 4 * a3 + 2 * a2 - 2 * a1;
        a1 = a2;   /* shift window */
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}

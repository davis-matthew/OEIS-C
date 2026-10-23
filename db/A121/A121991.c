/*
 * A121991  a(n) = 3*a(n-1) - a(n-2) - a(n-3) + 12.
 * https://oeis.org/A121991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) - a(n-2) - a(n-3) + 12
   with a(0) = 0, a(1) = 1, a(2) = 13, a(3) = 50 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 13;
    if (n == 3) return 50;

    /* Iteratively compute from a(0) to a(n). */
    seq_t a0 = 0, a1 = 1, a2 = 13, a3 = 50, ai;
    for (idx_t i = 4; i <= n; ++i) {
        ai = a3 * 3 - a2 - a1 + 12;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}

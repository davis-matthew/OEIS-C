/*
 * A102865  Base-4 digits are, in order, the first n terms of the sequence (1,
 *          3, 21, 203, 2021, 20203, 202021, 2020203, 20202021, 202020203, ...
 *          ).
 * https://oeis.org/A102865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 4*a(n-1) + a(n-2) - 4*a(n-3) with initial values a(0) =
 *            1, a(1) = 3, a(2) = 9
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices directly. */
    if (n == 0) return 1;
    if (n == 1) return 3;
    if (n == 2) return 9;

    /* Use the linear recurrence for all n ≥ 3. */
    seq_t a0 = 1;   /* a(n-3) */
    seq_t a1 = 3;   /* a(n-2) */
    seq_t a2 = 9;   /* a(n-1) */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i)
    {
        next = 4 * a2 + a1 - 4 * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}

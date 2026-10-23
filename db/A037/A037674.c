/*
 * A037674  Base-4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,2,3.
 * https://oeis.org/A037674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence: a(1) = 1;
 * a(n) = 4 * a(n-1) + d_n,
 * where the digit sequence d repeats 1,0,2,3, i.e.
 * d_n = {1,0,2,3}[ (n-1) mod 4 ].
 */
seq_t a(idx_t n)
{
    if (n <= 0)
        return 0;               /* offset 1, but guard against 0 */

    static const seq_t digits[4] = {1, 0, 2, 3};
    seq_t value = 1;              /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t d = digits[(i - 1u) & 3u];
        value = value * 4 + d;
    }

    return value;
}

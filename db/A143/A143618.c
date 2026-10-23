/*
 * A143618  Decimal expansion of 127/216.
 * https://oeis.org/A143618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-3) for n > 5; a(0)=5, a(1)=8, a(2)=7, a(3)=9, a(4)=6, a(5)=2 */
seq_t a(idx_t n)
{
    static const seq_t init[6] = {5, 8, 7, 9, 6, 2};
    static const seq_t cycle[3] = {9, 6, 2};

    if (n < 6)
        return init[n];

    /* n >= 6, cycle repeats every 3 */
    return cycle[(n - 6) % 3];
}

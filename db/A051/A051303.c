/*
 * A051303  Number of 3-element proper antichains of an n-element set.
 * https://oeis.org/A051303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 29*a(n-1) - 343*a(n-2) + 2135*a(n-3) - 7504*a(n-4) +
 *            14756*a(n-5) - 14832*a(n-6) + 5760*a(n-7) Initial values:
 *            a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=30, a(5)=605, a(6)=9030
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[7] = {0, 0, 0, 1, 30, 605, 9030};
    if (n < 7)
        return init[n];

    seq_t a0 = init[0];
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t a4 = init[4];
    seq_t a5 = init[5];
    seq_t a6 = init[6];
    for (idx_t i = 7; i <= n; ++i) {
        /* Compute the next term using the recurrence with
           only positive intermediate values to avoid unsigned
           underflow. */
        seq_t pos = 29 * a6 + 2135 * a4 + 14756 * a2 + 5760 * a0;
        seq_t neg = 343 * a5 + 7504 * a3 + 14832 * a1;
        seq_t next = pos - neg;
        a0 = a1; a1 = a2; a2 = a3; a3 = a4; a4 = a5; a5 = a6; a6 = next;
    }
    return a6;
}

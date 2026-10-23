/*
 * A130004  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+449)^2 = y^2.
 * https://oeis.org/A130004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) + 898 for n > 6, with a(1)=0, a(2)=31,
 *            a(3)=1204, a(4)=1347, a(5)=1504, a(6)=8151. This implementation
 *            computes the value by iterating the recurrence
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 29, u64 n <= 66, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values for n = 1..6 */
    static const seq_t init[6] = {0, 31, 1204, 1347, 1504, 8151};

    if (n <= 6)
        return init[n - 1];

    seq_t ring[6];
    for (int i = 0; i < 6; ++i)
        ring[i] = init[i];

    for (idx_t i = 7; i <= n; ++i)
    {
        idx_t idx   = (i - 1) % 6; /* position for a(i)       */
        idx_t idx3  = (i - 4) % 6; /* position for a(i-3)     */
        idx_t idx6  = (i - 7) % 6; /* position for a(i-6)     */

        ring[idx] = 6 * ring[idx3] - ring[idx6] + 898;
    }

    return ring[(n - 1) % 6];
}

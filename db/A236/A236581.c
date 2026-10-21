/*
 * A236581  The number of tilings of a 7 X (4n) floor with 1 X 4 tetrominoes.
 * https://oeis.org/A236581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence a(n) = 8*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4) with
 *            initial values a(0)=1, a(1)=5, a(2)=37, a(3)=269
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 5;
    if (n == 2) return 37;
    if (n == 3) return 269;

    seq_t a0 = 1;
    seq_t a1 = 5;
    seq_t a2 = 37;
    seq_t a3 = 269;
    idx_t i;

    for (i = 4; i <= n; ++i) {
        /* Compute a(i) = 8*a3 - 6*a2 + 4*a1 - a0
           Rearranged to avoid unsigned underflow by first
           adding the positive terms and then subtracting. */
        seq_t temp = a3 * 8 + a1 * 4;
        temp -= a2 * 6;
        temp -= a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = temp;
    }
    return a3;
}

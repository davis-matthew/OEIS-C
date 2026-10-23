/*
 * A107243  Sum of squares of pentanacci numbers (A001591).
 * https://oeis.org/A107243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0}^{n} F_5(k)^2 where F_5 satisfies F_5(i) =
 *            F_5(i-1)+F_5(i-2)+F_5(i-3)+F_5(i-4)+F_5(i-5) with initial values
 *            F_5(0)=F_5(1)=F_5(2)=F_5(3)=0, F_5(4)=1
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 21, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* Pentanacci values for indices 0..4 */
    seq_t f[5] = {0, 0, 0, 0, 1};

    /* Handle the initial 0..4 indices directly */
    if (n <= 4) {
        for (idx_t i = 0; i <= n; ++i) {
            sum += f[i] * f[i];
        }
        return sum;
    }

    /* Sum squares for indices 0..4 */
    for (idx_t i = 0; i <= 4; ++i) {
        sum += f[i] * f[i];
    }

    /* Compute for indices 5 .. n */
    for (idx_t k = 5; k <= n; ++k) {
        seq_t new_val = f[0] + f[1] + f[2] + f[3] + f[4];
        sum += new_val * new_val;
        /* Shift the pentanacci window */
        f[0] = f[1];
        f[1] = f[2];
        f[2] = f[3];
        f[3] = f[4];
        f[4] = new_val;
    }

    return sum;
}

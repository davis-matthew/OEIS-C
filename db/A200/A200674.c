/*
 * A200674  Total eccentricity of Tower of Hanoi graph H_n^{3} (divided by 3).
 * https://oeis.org/A200674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; for n>=0 let y[0]=0 and y[n+1] = 6*y[n] + 2*3^n + 3 -
 *            2*(n==0); a(n) = y[n] / 3. The recurrence correctly reproduces
 *            all terms of OEIS sequence A200674
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t y     = (seq_t)0;   /* y[n]  = 3 * a[n]  */
    seq_t pow3  = (seq_t)1;   /* 3^n  (starts at 3^0) */

    for (idx_t i = 0; i < n; ++i) {
        /* term = 2*3^i + 3; subtract 2 only for i==0 because 0^0 = 1 */
        seq_t term = (seq_t)2 * pow3 + (seq_t)3;
        if (i == 0) term -= (seq_t)2;   /* handle the -2*0^0 term */

        y = (seq_t)6 * y + term;       /* y_{i+1} = 6*y_i + term */
        pow3 *= (seq_t)3;              /* advance to 3^{i+1} */
    }

    return y / (seq_t)3;                /* a[n] = y[n] / 3 */
}

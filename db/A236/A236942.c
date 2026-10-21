/*
 * A236942  Number of (n+1) X (1+1) 0..3 arrays with the maximum of every 2 X
 *          2 subblock equal.
 * https://oeis.org/A236942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{m=0..3} f(n,m) * where f(n,m) counts the (n+1)×2
 *            arrays over {0,1,2,3} whose every * 2×2 subblock has maximum m.
 *            With H = 2m+1 (rows containing at least one m) * and L = m²
 *            (rows containing no m) * the DP recurrence is: * dp_H[0] = H,
 *            dp_L[0] = L * for i = 1..n: * dp_H[i] = (dp_H[i‑1] +...
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t M = 0; M < 4; ++M)
    {
        seq_t M2 = (seq_t)M * (seq_t)M;                /* L = m^2 */
        seq_t H  = (seq_t)2 * (seq_t)M + (seq_t)1;    /* H = 2*m+1 */

        seq_t A = H;            /* dp_H[0] */
        seq_t B = M2;           /* dp_L[0] */

        for (idx_t i = 1; i <= n; ++i)
        {
            seq_t newA = (A + B) * H;
            seq_t newB = A * M2;
            A = newA;
            B = newB;
        }

        seq_t f = A + B;
        total += f;
    }
    return total;
}

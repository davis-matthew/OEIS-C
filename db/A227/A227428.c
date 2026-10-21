/*
 * A227428  Number of twos in row n of triangle A083093.
 * https://oeis.org/A227428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binomial coefficients C(n,k) ≡ 2 (mod 3) (i.e.
 *            the number of twos in row n of Pascal's triangle modulo 3).
 *            Computed via Lucas' theorem for the prime 3
 * Range:     u8 n <= 727, u16 n <= 9669, u32 n <= 9669, u64 n <= 9669,
 *            u128 n <= 9669, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t cnt = 0;

    /* residues of small binomials C(a,b) mod 3 for a,b∈{0,1,2} */
    static const int small[3][3] = {
        {1, 0, 0},  /* C(0,0)=1 */
        {1, 1, 0},  /* C(1,0)=C(1,1)=1 */
        {1, 2, 1}   /* C(2,0)=C(2,2)=1 , C(2,1)=2 */
    };

    for (idx_t k = 0; k <= n; ++k) {
        idx_t nn = n;
        idx_t kk = k;
        int r = 1;
        while (nn || kk) {
            int ni = nn % 3;
            int ki = kk % 3;
            if (ki > ni) {          /* binomial is 0 */
                r = 0;
                break;
            }
            r = (r * small[ni][ki]) % 3;
            nn /= 3;
            kk /= 3;
        }
        if (r == 2) ++cnt;
    }

    return cnt;
}

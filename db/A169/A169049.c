/*
 * A169049  Number of reduced words of length n in Coxeter group on 36
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^24 = I.
 * https://oeis.org/A169049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of t^n in
 *            (1+2t+2t^2+…+2t^23+t^24)/(595*t^24-34*t^23-…-34*t+1) Recurrence:
 *            a(n) = N(n) + 34 * Σ_{i=1}^{min(n,23)} a(n-i) - (n>=24 ? 595 *
 *            a(n-24) : 0), where N(n) = 1 for n=0 or 24, N(n)=2 for 1≤n≤23,
 *            else 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t buf[24] = {0};
    seq_t res = 0;
    for (idx_t i = 0; i <= n; ++i) {
        seq_t sum = 0;
        if (i > 0) {
            idx_t limit = i < 23 ? i : 23;
            for (idx_t j = 1; j <= limit; ++j) {
                idx_t idx = (i + 24 - j) % 24;  /* (i-j) modulo 24 */
                sum += (seq_t)34 * buf[idx];
            }
        }
        if (i >= 24) {
            idx_t idx24 = i % 24;            /* (i-24) modulo 24 */
            sum -= (seq_t)595 * buf[idx24];
        }

        seq_t coeffN;
        if (i == 0 || i == 24)
            coeffN = 1;
        else if (i <= 23)
            coeffN = 2;
        else
            coeffN = 0;

        res = coeffN + sum;
        buf[i % 24] = res;
    }
    return res;
}

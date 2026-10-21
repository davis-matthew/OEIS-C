/*
 * A204217  G.f.: Sum_{n>=1} n * x^(n*(n+1)/2) / (1 - x^n).
 * https://oeis.org/A204217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k≥1, T_k≤n, (n–T_k)%k==0} k where T_k = k(k+1)/2
 * Range:     u8 n <= 357, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t sum = 0;
    for (idx_t k = 1; ; ++k) {
        seq_t kk = (seq_t)k;                 /* cast for seq_t arithmetic */
        seq_t T;
        if (k & 1) {                         /* k odd: (k+1)/2 is an integer */
            T = kk * ((kk + 1) / 2);
        } else {                             /* k even */
            T = (kk / 2) * (kk + 1);
        }
        if (T > (seq_t)n) break;
        seq_t rem = (seq_t)n - T;
        if (rem % kk == 0) {
            sum += kk;
        }
    }
    return sum;
}

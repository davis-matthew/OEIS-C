/*
 * A098534  Mod 3 analog of Stern's diatomic series.
 * https://oeis.org/A098534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/2)} C(n-k-1,k) modulo 3
 * Range:     u8 n <= 4374, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 8932 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline int binom_mod3(idx_t N, idx_t K)
{
    /* Small table of binomial coefficients modulo 3 for single base‑3 digits.   */
    static const int smallC[3][3] = {
        {1,0,0},
        {1,1,0},
        {1,2,1}
    };
    int result = 1;
    while (N || K) {
        int nd = N % 3;
        int kd = K % 3;
        if (kd > nd)
            return 0;
        result = (result * smallC[nd][kd]) % 3;
        N /= 3;
        K /= 3;
    }
    return result;
}
/* The result is accumulated as a plain integer sum of the residues.   */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t limit = (n - 1) / 2;          /* floor((n-1)/2) */
    seq_t sum = 0;
    for (idx_t k = 0; k <= limit; ++k) {
        idx_t N = n - k - 1;            /* N = n-k-1 >= k for k <= limit */
        int r = binom_mod3(N, k);
        sum += (seq_t)r;
    }
    return sum;
}

/*
 * A052766  Expansion of e.g.f.: (log(1-x))^2*x^3.
 * https://oeis.org/A052766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n*(n-1)*(n-2)*abs(Stirling1(n-3, 2)), with
 *            a(0)=a(1)=a(2)=a(3)=a(4)=0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first non‑zero term occurs at n = 5 */
    if (n < 5)
        return 0;

    /* m = n-3; compute |S(m,2)| (unsigned Stirling number of the first kind) */
    idx_t m = n - 3;
    seq_t s_prev = 1;          /* S(2,2) = 1 */
    seq_t fact   = 1;          /* (0)! for the first iteration (i=2) */

    if (m > 2) {
        /* iterate i = 3 .. m */
        for (idx_t i = 3; i <= m; ++i) {
            seq_t new_s = fact + (seq_t)(i - 1) * s_prev;
            fact = fact * (seq_t)(i - 1);  /* fact becomes (i-1)! for next step */
            s_prev = new_s;
        }
    }

    /* result = 2 * n * (n-1) * (n-2) * S(m,2) */
    seq_t res = (seq_t)2;
    res *= (seq_t)n;
    res *= (seq_t)(n - 1);
    res *= (seq_t)(n - 2);
    res *= s_prev;
    return res;
}

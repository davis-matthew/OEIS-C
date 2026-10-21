/*
 * A089693  Numbers k such that phi(k) = 2^bigomega(k).
 * https://oeis.org/A089693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=3, a(3)=10, a(4)=20, a(5)=30; * a(2*m)=5*2^m for
 *            m≥2, * a(2*m+1)=15*2^(m-1) for m≥2
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 59, u64 n <= 123,
 *            u128 n <= 251, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)10;
    if (n == 4) return (seq_t)20;
    if (n == 5) return (seq_t)30;

    /* n ≥ 6 */
    if ((n & 1u) == 0) {               /* even n = 2*m, m ≥ 3 */
        idx_t m = n >> 1u;
        seq_t pow2 = (seq_t)1 << m;    /* 2^m */
        return (seq_t)5 * pow2;
    } else {                           /* odd n = 2*m+1, m ≥ 3 */
        idx_t m = (n - 1u) >> 1u;
        seq_t pow2 = (seq_t)1 << (m - 1u);  /* 2^(m-1) */
        return (seq_t)15 * pow2;
    }
}

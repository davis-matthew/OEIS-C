/*
 * A085432  Erroneous duplicate of A023022.
 * https://oeis.org/A085432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = φ(n)/2 (Euler's totient divided by 2), with the *
 *            following exceptional values: a(1)=1, a(2)=1, a(7)=2. * a(1)=1,
 *            a(2)=1, a(3)=1, a(4)=1, a(5)=2, a(6)=1, a(7)=2, a(8)=2, *
 *            a(9)=3, a(10)=2, a(11)=5, a(12)=2, a(13)=6, a(14)=3, a(15)=4, *
 *            a(16)=4, a(17)=8, a(18)=3, a(19)=9, a(20)=4, a(21)=6,...
 * Range:     i8 n <= 24, i16 n <= 24, i32 n <= 24, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special small values that deviate from the φ(n)/2 rule */
    if (n <= 2)
        return 1;
    if (n == 7)
        return 2;

    idx_t m = n;               /* Working copy for factorisation */
    seq_t phi = (seq_t)n;      /* φ(n) will be computed here */

    /* Factorise m and compute φ(m) multiplicatively */
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            /* p is a prime divisor of m */
            while (m % p == 0)
                m /= p;
            phi -= phi / (seq_t)p;
        }
    }
    if (m > 1)          /* Remaining prime factor > sqrt(original n) */
        phi -= phi / (seq_t)m;

    return phi / 2;
}

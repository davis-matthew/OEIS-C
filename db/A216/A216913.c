/*
 * A216913  a(n) = Gauss_primorial(3*n, 3) / Gauss_primorial(3*n, 3*n).
 * https://oeis.org/A216913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product of the distinct prime divisors of n that are not
 *            3 (i.e. a(n) = Π_{p|n, p≠3} p ). This follows from the
 *            multiplicative definition a(p^e)=p for p≠3 and a(3^e)=1
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 1;
    seq_t result = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            if (p != 3) result *= (seq_t)p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) { /* remaining prime factor */
        if (m != 3) result *= (seq_t)m;
    }
    return result;
}

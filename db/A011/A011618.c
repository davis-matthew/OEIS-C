/*
 * A011618  Legendre symbol (n,179).
 * https://oeis.org/A011618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = n^89 (mod 179) (Legendre symbol (n,179). Values are 0, 1,
 *            or -1.)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t MOD = 179;
    idx_t b = n % MOD;
    if (b == 0) return (seq_t)0;
    idx_t exp = 89;
    idx_t res = 1;
    while (exp) {
        if (exp & 1) res = (res * b) % MOD;
        b = (b * b) % MOD;
        exp >>= 1;
    }
    return (res == 1) ? (seq_t)1 : (seq_t)-1;
}

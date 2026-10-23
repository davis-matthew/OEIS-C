/*
 * A011597  Legendre symbol (n,71).
 * https://oeis.org/A011597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n,71) = n^35 (mod 71)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const unsigned long long MOD = 71ULL;
    unsigned long long k = (unsigned long long)n % MOD;
    if (k == 0ULL)
        return 0;
    unsigned long long exp = 35ULL;
    unsigned long long result = 1ULL;
    unsigned long long base = k;
    while (exp) {
        if (exp & 1ULL)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1ULL;
    }
    if (result == 70ULL)
        return (seq_t)-1;
    return (seq_t)result;
}

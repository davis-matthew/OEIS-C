/*
 * A011598  Legendre symbol (n,73).
 * https://oeis.org/A011598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n,73). Compute as n^36 mod 73; 0 if n ≡
 *            0 (mod 73), 1 if n is a quadratic residue mod 73, -1 otherwise
 * Range:     i8 n <= 1773, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = 73;
    seq_t m = (seq_t)n % MOD;
    if (m == 0)
        return (seq_t)0;

    seq_t res = 1 % MOD;
    seq_t base = m % MOD;
    uint64_t exp = 36;
    while (exp) {
        if (exp & 1ULL)
            res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1ULL;
    }
    return (res == 1) ? (seq_t)1 : (seq_t)-1;
}

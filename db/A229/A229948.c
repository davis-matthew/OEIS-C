/*
 * A229948  Decimal expansion of 2187/2048, the Pythagorean apotome.
 * https://oeis.org/A229948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy, fini, full
 * Formula:   a(n) = floor((2187/2048) * 10^(n-1)) % 10
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th decimal digit (a(1) is the integer part). */

static unsigned long long modpow10(idx_t exp)
{
    unsigned long long res  = 1ULL;
    unsigned long long base = 10ULL;
    const unsigned long long MOD = 20480ULL;          /* 10 * 2048 */

    while (exp > 0)
    {
        if (exp & 1ULL)
            res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)                       /* safety: n should be >= 1 */
        return 0;

    unsigned long long pow10_mod = modpow10(n - 1);   /* 10^(n-1) mod 20480 */
    unsigned long long prod     = 2187ULL * pow10_mod;
    unsigned long long xmod     = prod % 20480ULL;    /* X mod 20480 */
    unsigned long long digit    = xmod / 2048ULL;    /* 0 … 9 */

    return (seq_t)digit;
}

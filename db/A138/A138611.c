/*
 * A138611  6^n mod 4^n.
 * https://oeis.org/A138611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6^n mod 4^n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_mod_seq(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1;
    seq_t b = base % mod;          /* initial base */
    while (exp > 0)
    {
        if (exp & 1u)
            result = (result * b) % mod;
        exp >>= 1u;
        if (exp)                 /* avoid unnecessary multiplication on last loop */
            b = (b * b) % mod;
    }
    return result;
}

static seq_t pow2_seq(idx_t e)
{
    seq_t res = 1;
    seq_t two = 2;
    while (e > 0)
    {
        if (e & 1u)
            res *= two;
        two *= two;
        e >>= 1u;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0u)
        return 0;

    /* modulus = 2^n  (= 4^(n/2) when n even) */
    seq_t mod = pow2_seq(n);          /* 2^n */

    /* r = 3^n mod 2^n */
    seq_t r = pow_mod_seq((seq_t)3, n, mod);

    /* a(n) = 2^n * r = mod * r */
    return r * mod;
}

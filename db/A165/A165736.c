/*
 * A165736  a(n) = n^n^n^n^n^n^n^n^n^n^... read mod 10^10.
 * https://oeis.org/A165736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   a(n) = n^(n^(n^(...))) read modulo 10^10
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute modular exponentiation with 128‑bit intermediate */
static seq_t powmod(seq_t base, seq_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1)
            result = (seq_t)((unsigned __int128)result * base % mod);
        base = (seq_t)((unsigned __int128)base * base % mod);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* if base is a multiple of 10 the infinite tower is a multiple of 10,
       therefore the result is 0.  This special case fixes the hidden
       0‑valuation bug that caused a(10000) to be computed as 1. */
    if (n % 10 == 0)
        return 0;

    seq_t base = (seq_t)n;
    seq_t res  = powmod(base, base, 10);          /* n^n mod 10   */
    seq_t mod  = 10;
    for (int k = 2; k <= 10; ++k)
    {
        mod *= 10;                                 /* 10^k         */
        res = powmod(base, res, mod);              /* n^res mod 10^k */
    }
    return res;
}

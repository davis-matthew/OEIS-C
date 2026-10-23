/*
 * A036165  Log base 2 (n) mod 29.
 * https://oeis.org/A036165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the exponent k (0 ≤ k < 29) with 2^k ≡ n (mod 29); if n ≡
 *            0 (mod 29) the function returns 0
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t MOD = 29ULL;
    idx_t m = n % MOD;
    if (m == 0)          /* n divisible by 29 – treat as 0 */
        return 0;

    idx_t exp = 0;
    idx_t val = 1;        /* 2^0 mod MOD */
    do
    {   if (val == m) break;
        val = (val * 2) % MOD;
        ++exp;
    } while (exp < MOD);  /* guard against overflow, though a match surely occurs */

    return (seq_t)exp;
}

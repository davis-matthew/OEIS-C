/*
 * A021459  Decimal expansion of 1/455.
 * https://oeis.org/A021459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = floor(10^(n+1) / 455) mod 10 (decimal digits of 1/455) */
seq_t a(idx_t n)
{
    /* compute 10^(n+1) modulo 4550 (= 455 * 10) */
    const seq_t MOD = 4550;
    seq_t result = 1 % MOD;
    seq_t base   = 10 % MOD;
    idx_t exp    = n + 1;  /* exponent = n+1 */

    while (exp > 0)
    {
        if (exp & 1ULL)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }

    /* the digit is floor(result / 455)   (result < 4550, so 0‑9) */
    return result / 455;
}

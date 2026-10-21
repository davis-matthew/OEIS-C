/*
 * A078684  a(n) = 3^floor(n^2/4).
 * https://oeis.org/A078684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^floor(n^2/4)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the exponent e = floor(n²/4) with 128‑bit arithmetic
   to avoid overflow on 64‑bit indices, then use exponentiation
   by squaring to produce 3^e in seq_t. */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1u)
            result = result * base;
        base = base * base;
        exp >>= 1u;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* e = floor(n * n / 4) computed in 128‑bit to avoid overflow. */
    idx_t e = ( ( (__uint128_t)n * n ) / 4 );
    return pow_seq_t((seq_t)3, e);
}

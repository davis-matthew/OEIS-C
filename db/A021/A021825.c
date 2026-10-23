/*
 * A021825  Decimal expansion of 1/821.
 * https://oeis.org/A021825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10 * (10^n mod 821) / 821)
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the (n‑th) digit after the decimal point in the expansion of 1/821. */
seq_t a(idx_t n)
{
    /* Compute remainder r = 10^n mod 821 using binary exponentiation. */
    seq_t r      = 1;          /* 10^0 mod 821 */
    seq_t base   = 10;         /* current base power of 10 */
    idx_t   exp  = n;
    const seq_t MOD = 821;     /* modulus for the remainder */

    while (exp)
    {
        if (exp & 1)
            r = (r * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }

    /* Now r = 10^n mod 821.
       The digit at position n is floor(10 * r / 821). */
    seq_t digit = (r * 10) / MOD;
    return digit;
}

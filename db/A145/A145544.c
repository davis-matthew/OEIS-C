/*
 * A145544  a(n) = 4*(4^n-3^n).
 * https://oeis.org/A145544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(4^n-3^n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fast modular exponentiation adapted for arbitrary‑precision seq_t */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0)
    {
        if (exp & 1ULL)
            result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 4^n and 3^n – using exponentiation by squaring */
    seq_t pow4 = pow_seq((seq_t)4, n);
    seq_t pow3 = pow_seq((seq_t)3, n);

    /* 4*(4^n - 3^n) */
    return (pow4 - pow3) * 4;
}

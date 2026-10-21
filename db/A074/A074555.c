/*
 * A074555  a(n) = 3^n + 6^n + 7^n.
 * https://oeis.org/A074555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n + 6^n + 7^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// Helper: fast exponentiation by squaring for seq_t
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;        // identity for multiplication
    seq_t b = base;          // current power of base

    while (exp > 0)
    {
        if (exp & 1)
            result *= b;     // multiply result by current power
        b *= b;              // square base for next bit
        exp >>= 1;           // shift to next bit of exponent
    }
    return result;
}

seq_t a(idx_t n)
{
    return pow_seq(3, n) + pow_seq(6, n) + pow_seq(7, n);
}

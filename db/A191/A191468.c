/*
 * A191468  a(n) = 8^n - 5^n.
 * https://oeis.org/A191468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n - 5^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t power_u64(idx_t exp, seq_t base)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp)
    {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    return power_u64(n, (seq_t)8) - power_u64(n, (seq_t)5);
}

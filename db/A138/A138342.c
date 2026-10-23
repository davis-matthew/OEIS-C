/*
 * A138342  First differences of A007088.
 * https://oeis.org/A138342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = binary_to_decimal(n) - binary_to_decimal(n-1) where
 *            binary_to_decimal(n) = Σ bit_i(n) * 10^i, i = 0 .. floor(log2 n)
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 1023, u64 n <= 9828,
 *            u128 n <= 9828, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binary_to_decimal(idx_t x)
{
    seq_t result = 0;
    seq_t power   = 1;   /* 10^0 */
    idx_t mask    = x;

    while (mask != 0)
    {
        if (mask & 1) result += power;
        power *= 10;
        mask >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* The index starts at 1.  n-1 is safe because n >= 1. */
    return binary_to_decimal(n) - binary_to_decimal(n - 1);
}

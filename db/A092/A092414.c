/*
 * A092414  Negative of the determinant of the 3 X 3 matrix with entries
 *          (X+Y)^n.
 * https://oeis.org/A092414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 50^n + 54^n - 48^n + 64^n - 2*60^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p50 = pow_seq((seq_t)50, n);
    seq_t p54 = pow_seq((seq_t)54, n);
    seq_t p48 = pow_seq((seq_t)48, n);
    seq_t p64 = pow_seq((seq_t)64, n);
    seq_t p60 = pow_seq((seq_t)60, n);

    return p50 + p54 - p48 + p64 - 2 * p60;
}

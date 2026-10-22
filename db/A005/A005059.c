/*
 * A005059  a(n) = (5^n - 3^n)/2.
 * https://oeis.org/A005059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5^n - 3^n)/2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t b, idx_t e)
{
    seq_t result = (seq_t)1;
    seq_t base = b;
    while (e > 0) {
        if (e & 1) {
            result *= base;
        }
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t five_n = pow_seq((seq_t)5, n);
    seq_t three_n = pow_seq((seq_t)3, n);
    return (five_n - three_n) / 2;
}

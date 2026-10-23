/*
 * A013794  a(n) = 11^(4n+1).
 * https://oeis.org/A013794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^(4n+1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(const seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    idx_t e = exp;
    while (e != 0) {
        if (e & 1) result *= b;
        b *= b;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t exp = n * 4 + 1;
    return pow_seq((seq_t)11, exp);
}

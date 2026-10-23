/*
 * A137786  a(n) = 4^n - 3^n - 2^n.
 * https://oeis.org/A137786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4^n - 3^n - 2^n
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 15, i64 n <= 31, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_u64(idx_t exp, seq_t base)
{
    seq_t result = (seq_t)1;
    seq_t b = base;
    while (exp)
    {
        if (exp & (idx_t)1)
            result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p4 = pow_seq_u64(n, (seq_t)4);
    seq_t p3 = pow_seq_u64(n, (seq_t)3);
    seq_t p2 = pow_seq_u64(n, (seq_t)2);
    return p4 - p3 - p2;
}

/*
 * A196731  Expansion of g.f. (1-x)/(1-12*x).
 * https://oeis.org/A196731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(n)=11*12^(n-1) for n>0 (recurrence: a(1)=11,
 *            a(n)=12*a(n-1) for n>1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_uint_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = base;
    while (exp) {
        if (exp & (idx_t)1) result *= b;
        exp >>= 1;
        if (exp) b *= b;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    return ((seq_t)11) * pow_uint_seq((seq_t)12, n - 1);
}

/*
 * A166024  Define dsf(n) = A045503(n) = n_1^{n_1}+n_2^{n_2}+n_3^{n_3} +
 *          n_m^{n_m}, where {n_1,n_2,n_3,...n_m} is the list of the decimal
 *          digits of n. Starting with a(1) = 421845123, a(n+1) = dsf(a(n)).
 * https://oeis.org/A166024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (n is odd) ? 421845123 : 16780890 (this follows from the
 *            recurrence a(n+1)=dsf(a(n)) with a(1)=421845123)
 * Range:     u8 none, u16 none, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t dsf(seq_t x)
{
    seq_t sum = 0;
    while (x > 0)
    {
        seq_t digit = x % 10;
        seq_t pow = 1;
        for (seq_t i = 0; i < digit; ++i)
            pow *= digit;
        sum += pow;
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* a(1) is given; for n>1 the sequence alternates between two values. */
    if ((n & 1) == 1)          /* odd n */
        return (seq_t)421845123;
    else                       /* even n */
        return dsf((seq_t)421845123);
}

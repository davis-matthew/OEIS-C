/*
 * A021996  Decimal expansion of 1/992.
 * https://oeis.org/A021996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth decimal digit of 1/992 (0-indexed)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t i;
    unsigned long long rem = 1;
    unsigned long long digit;
    for (i = 0; i <= n; ++i)
    {
        rem *= 10ULL;
        digit = rem / 992ULL;
        rem  %= 992ULL;
        if (i == n)
            return (seq_t)digit;
    }
    return (seq_t)0; /* unreachable */
}

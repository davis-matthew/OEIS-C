/*
 * A051746  a(n) = n(n+7)(n+1)(n^2+2n+12)/120.
 * https://oeis.org/A051746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+7)*(n+1)*(n^2+2*n+12)/120
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 218, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned long long fn = (unsigned long long)n;
    unsigned long long prod = fn * (fn + 7ULL) * (fn + 1ULL);
    unsigned long long part = fn * fn + 2ULL * fn + 12ULL;
    prod *= part;
    prod /= 120ULL;
    return (seq_t)prod;
}

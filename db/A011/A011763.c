/*
 * A011763  Days in year in proleptic Gregorian calendar.
 * https://oeis.org/A011763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 366 if (n%4==0 and (n%100!=0 or n%400==0)) else 365
 * Range:     u8 none, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n % 4ULL) != 0ULL)
        return (seq_t)365;
    if ((n % 100ULL) != 0ULL)
        return (seq_t)366;
    if ((n % 400ULL) == 0ULL)
        return (seq_t)366;
    return (seq_t)365;
}

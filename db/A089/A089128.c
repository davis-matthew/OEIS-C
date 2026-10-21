/*
 * A089128  a(n) = gcd(6,n).
 * https://oeis.org/A089128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = gcd(6,n)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Since gcd(6,n) is 6,3,2 or 1, we use simple checks.
       The modulo arguments use the idx_t type (unsigned). */
    if ((n % 6ULL) == 0ULL)
        return (seq_t)6;
    if ((n % 3ULL) == 0ULL)
        return (seq_t)3;
    if ((n % 2ULL) == 0ULL)
        return (seq_t)2;
    return (seq_t)1;
}

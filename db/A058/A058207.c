/*
 * A058207  Three steps forward, two steps back.
 * https://oeis.org/A058207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = ( n + 4*((n mod 5) mod 4) + 6*floor((n mod 5)/4) ) / 5
 * Range:     u8 n <= 1174, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* remainder r = n mod 5 */
    seq_t r = n % 5;
    /* numerator = n + 4*((r mod 4)) + 6*floor(r/4) */
    seq_t numer = (seq_t)n + 4 * (r % 4) + 6 * (r / 4);
    return numer / 5;
}

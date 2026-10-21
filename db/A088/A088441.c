/*
 * A088441  a(n) = n if n == 0 (mod 3), a(n) = 1 if n == 2 (mod 3), otherwise
 *          a(n) = floor((n-2)/2).
 * https://oeis.org/A088441
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n mod 3 = 0, 1 if n mod 3 = 2, floor((n-2)/2)
 *            otherwise (offset = 2)
 * Range:     u8 n <= 257, u16 n <= 5002, u32 n <= 5002, u64 n <= 5002,
 *            u128 n <= 5002, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 2 the value is undefined; return 0 (offset starts at 2) */
    if (n < 2) {
        return (seq_t)0;
    }

    seq_t val = (seq_t)n;
    idx_t r = n % 3;          /* remainder modulo 3 */

    if (r == 0) {
        return val;           /* n if multiple of 3 */
    }
    if (r == 2) {
        return (seq_t)1;      /* 1 if remainder is 2 */
    }
    /* remainder must be 1: return floor((n-2)/2) */
    return (val - (seq_t)2) / (seq_t)2;
}

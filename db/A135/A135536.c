/*
 * A135536  a(n) = 8*a(n-2), with a(0) = 7, a(1) = 14.
 * https://oeis.org/A135536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-2), a(0)=7, a(1)=14
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)7;
    if (n == 1) return (seq_t)14;

    idx_t k = n >> 1;          /* floor(n/2) */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)8;
    idx_t exp    = k;

    /* binary exponentiation: result = 8^k */
    while (exp)
    {
        if (exp & 1ULL) result = result * base;
        base = base * base;
        exp >>= 1;
    }

    result = result * (seq_t)7;
    if (n & 1ULL) result = result * (seq_t)2;  /* odd n: multiply by 2 */

    return result;
}

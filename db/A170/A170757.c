/*
 * A170757  Expansion of g.f.: (1+x)/(1-37*x).
 * https://oeis.org/A170757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n>0, a(n) = 38 * 37^(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* binary exponentiation of 37^(n-1) */
    idx_t e = n - 1;          /* n > 0, safe to subtract 1 */
    seq_t result = 1;         /* 37^0 */
    seq_t base   = 37;        /* base */

    while (e > 0) {
        if (e & 1)
            result *= base;
        base *= base;
        e >>= 1;
    }
    return result * 38;
}

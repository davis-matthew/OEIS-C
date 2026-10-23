/*
 * A153499  a(n) is the number whose binary expansion is A153500(n).
 * https://oeis.org/A153499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4^n-1)/3 if n is even; For odd n > 1, a(n) = [(4^n-1)/3]
 *            XOR (mask), where mask has 1s in the consecutive positions from
 *            (n+1)/2 to (3*n-5)/2 inclusive
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base value (4^n-1)/3: 4^n = 1 << (2*n) */
    seq_t base = (((seq_t)1 << (2 * n)) - 1) / 3;

    /* odd n > 1 require a mask */
    if (n > 1 && (n & 1)) {
        idx_t start = (n + 1) / 2;              /* lower bound of mask */
        idx_t end   = (3 * n - 5) / 2;          /* upper bound of mask */

        seq_t high = (((seq_t)1 << (end + 1)) - 1);  /* bits 0..end */
        seq_t low  = (((seq_t)1 << start) - 1);       /* bits 0..start-1 */

        seq_t mask = high ^ low;               /* bits start..end */

        base ^= mask;                          /* apply the mask */
    }

    return base;
}

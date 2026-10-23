/*
 * A014928  a(1)=1, a(n) = n*13^(n-1) + a(n-1).
 * https://oeis.org/A014928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + n*13^(n-1), a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                      /* undefined for n < 1, but guard anyway       */
    }

    seq_t result = 1;                   /* a(1) */
    if (n == 1) {
        return result;
    }

    seq_t pow = 1;                      /* pow will hold 13^(k-1) starting with 1      */
    for (idx_t k = 2; k <= n; ++k) {
        pow *= (seq_t)13;               /* pow = 13^(k-1) after the multiplication   */
        result += (seq_t)k * pow;       /* add k*13^(k-1) to the accumulated sum     */
    }
    return result;
}

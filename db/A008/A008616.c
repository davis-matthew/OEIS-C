/*
 * A008616  Expansion of 1/((1-x^2)(1-x^5)).
 * https://oeis.org/A008616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + (n%2==0 ? 10 : 5)) / 10 This is equivalent to a(n) =
 *            floor((2*n + (n%2==0?20:10)) / 20), the closed‑form given in the
 *            OEIS entry
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use unsigned arithmetic; idx_t is uint64_t (unsigned) */
    if ((n & 1) == 0)          /* n is even */
        return (n + 10) / 10;
    else                       /* n is odd  */
        return (n + 5)  / 10;
}

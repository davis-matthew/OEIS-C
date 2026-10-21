/*
 * A065332  3-smooth numbers in their natural position, gaps filled with 0.
 * https://oeis.org/A065332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = n if n is of the form 2^i * 3^j, otherwise 0
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* safety for out-of-range index */
    idx_t t = n;
    /* Remove all factors of 2 */
    while ((t & 1u) == 0) t >>= 1;
    /* Remove all factors of 3 */
    while (t % 3 == 0) t /= 3;
    /* If nothing remains, n is 3‑smooth */
    if (t == 1) return (seq_t)n;
    return (seq_t)0;
}

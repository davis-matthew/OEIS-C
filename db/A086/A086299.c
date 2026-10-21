/*
 * A086299  a(n) = if n is 7-smooth then 1 else 0: characteristic function of
 *          7-smooth numbers.
 * https://oeis.org/A086299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 if n is 7‑smooth (all prime factors ≤ 7), otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safe guard, though n ≥ 1 in usage */

    idx_t m = n;

    /* Remove all powers of 2 */
    while ((m & 1ULL) == 0)   /* faster test for divisibility by 2 */
        m >>= 1;

    /* Remove all powers of 3 */
    while (m % 3 == 0)
        m /= 3;

    /* Remove all powers of 5 */
    while (m % 5 == 0)
        m /= 5;

    /* Remove all powers of 7 */
    while (m % 7 == 0)
        m /= 7;

    return (m == 1) ? (seq_t)1 : (seq_t)0;
}

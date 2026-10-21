/*
 * A065333  Characteristic function of 3-smooth numbers, i.e., numbers of the
 *          form 2^i*3^j (i, j >= 0).
 * https://oeis.org/A065333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = 1 if n is a product of powers of 2 and 3 (a 3‑smooth
 *            number), otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Work on a copy of n to avoid side effects. */
    idx_t x = n;
    /* Remove all factors of 2. */
    while ((x & 1) == 0)            /* faster than x % 2 */
        x >>= 1;
    /* Remove all factors of 3. */
    while (x % 3 == 0)
        x /= 3;
    /* If the remaining value is 1, n was 3‑smooth. */
    return (seq_t)(x == 1 ? 1 : 0);
}

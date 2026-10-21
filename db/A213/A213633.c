/*
 * A213633  [A000027/A007978], where [ ] = floor.
 * https://oeis.org/A213633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n / m ), where m is the smallest positive integer
 *            * that does NOT divide n (the least non‑divisor of n)
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 (OEIS offset 1). */
    idx_t m;
    /* The least non‑divisor is always > 1 because 1 divides every integer. */
    for (m = 2; ; ++m)
    {
        if (n % m != 0)
            break;
    }
    /* Perform division in seq_t to avoid overflow in intermediate results. */
    return (seq_t)n / (seq_t)m;
}

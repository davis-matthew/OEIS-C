/*
 * A058321  Number of x such that phi(x) = 2^n.
 * https://oeis.org/A058321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of x such that phi(x) = 2^n a(n) = n + 2 for n ≤ 31 a(n)
 *            = 32 for n ≥ 32 (assuming only the five known Fermat primes)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is unsigned, so comparison with 32 is safe */
    if (n < 32u)
        return (seq_t)(n + (seq_t)2);
    return (seq_t)32;
}

/*
 * A070532  a(n) = n^4 mod 14.
 * https://oeis.org/A070532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 14
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 14 first to keep intermediate values small */
    uint64_t r = (uint64_t)n % 14ULL;          /* n mod 14 */
    uint64_t r2 = r * r;                      /* r^2   */
    uint64_t r4 = r2 * r2;                    /* r^4   */
    return (seq_t)(r4 % 14ULL);                /* n^4 mod 14 */
}

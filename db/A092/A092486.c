/*
 * A092486  Take natural numbers, exchange first and third quadrisection.
 * https://oeis.org/A092486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n+3 for n≡0 (mod 4); a(n) = n+1 for n≡1 or 3 (mod 4);
 *            a(n) = n−1 for n≡2 (mod 4)
 * Range:     u8 n <= 254, u16 n <= 9764, u32 n <= 9764, u64 n <= 9764,
 *            u128 n <= 9764, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n & 3)         /* n % 4 */
    {
        case 0: return (seq_t)n + 3; /* 4k       → 4k+3 */
        case 1: return (seq_t)n + 1; /* 4k+1     → 4k+2 */
        case 2: return (seq_t)n - 1; /* 4k+2     → 4k+1 */
        case 3: return (seq_t)n + 1; /* 4k+3     → 4k+4 */
    }
    /* unreachable */
    return 0;
}

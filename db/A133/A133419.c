/*
 * A133419  Image of n under one application of the "5x+1" map.
 * https://oeis.org/A133419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = apply the "5x+1" map once: if n is even -> n/2, else if n
 *            ≡ 0 (mod 3) -> n/3, else -> 5*n + 1
 * Range:     u8 n <= 52, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & (idx_t)1) == 0)
        return (seq_t)n / 2;
    else if (n % 3 == 0)
        return (seq_t)n / 3;
    else
        return (seq_t)5 * (seq_t)n + (seq_t)1;
}

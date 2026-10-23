/*
 * A032777  Floor( n(n+1)(n+2)...(n+7) / (n+(n+1)+(n+2)+...+(n+7)) ).
 * https://oeis.org/A032777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( n*(n+1)*(n+2)*(n+3)*(n+4)*(n+5)*(n+6)*(n+7) /
 *            (8*n+28) )
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 28, u64 n <= 757, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a larger integer type for intermediate values to avoid overflow
       when seq_t is 64‑bit.  The result fits into seq_t as required. */
    unsigned __int128 prod = 1;
    for (idx_t i = 0; i < 8; ++i)
        prod *= (unsigned __int128)(n + i);

    unsigned __int128 sum = 8 * (unsigned __int128)n + 28;

    return (seq_t)(prod / sum);
}

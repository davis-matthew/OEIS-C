/*
 * A070687  n^6 mod 47.
 * https://oeis.org/A070687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 47
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (n mod 47)^6 mod 47.  Since 47 is small this fits in any native
       unsigned integer type used for seq_t. */
    uint64_t m = (uint64_t)n % 47ULL;
    uint64_t r = 1;
    for (int i = 0; i < 6; ++i)
        r = (r * m) % 47ULL;
    return (seq_t)r;
}

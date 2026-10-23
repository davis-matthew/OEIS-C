/*
 * A105337  a(n) = n*(n+1)/2 mod 256.
 * https://oeis.org/A105337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2 mod 256
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* reduce the index modulo 512 – the sequence has period 512 */
    idx_t r = n & 511ULL;
    /* compute the triangular number using a wider native type to avoid overflow */
    uint64_t prod = (uint64_t)r * ((uint64_t)r + 1ULL);
    uint64_t tri   = prod / 2ULL;
    /* keep only the lower 8 bits */
    return (seq_t)(tri & 0xFFULL);
}

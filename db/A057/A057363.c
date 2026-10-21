/*
 * A057363  a(n) = floor(8*n/13).
 * https://oeis.org/A057363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(8*n/13)
 * Range:     u8 n <= 415, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit product to avoid overflow when n is near UINT64_MAX.
       The result fits into 64 bits, so we can cast back safely. */
    unsigned __int128 prod = (unsigned __int128)n * 8;
    uint64_t res = (uint64_t)(prod / 13);
    return (seq_t)res;
}

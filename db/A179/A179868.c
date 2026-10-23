/*
 * A179868  (Number of 1's in the binary expansion of n) mod 4.
 * https://oeis.org/A179868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = popcount(n) mod 4
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of 1‑bits in the binary expansion of n,
   then takes the result modulo 4. */

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t x = n; x != 0; x >>= 1) {
        count += x & 1;   /* add the least significant bit */
    }
    return count % 4;
}

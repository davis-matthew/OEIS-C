/*
 * A071858  (Number of 1's in binary expansion of n) mod 3.
 * https://oeis.org/A071858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (popcount(n)) mod 3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the number of 1's in the binary expansion of n and
 * return that count modulo 3.  No memoisation is required
 * because the computation is O(log n). */
seq_t a(idx_t n)
{
    seq_t cnt = 0;              /* count of 1-bits (fits in seq_t) */
    idx_t x = n;                /* use idx_t for shifting safely */
    while (x) {                 /* loop until all bits are processed */
        cnt += x & 1;           /* add least significant bit if it is 1 */
        x >>= 1;                /* shift right to examine next bit */
    }
    return (seq_t)(cnt % 3);    /* result is always 0, 1, or 2 */
}

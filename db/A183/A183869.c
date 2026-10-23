/*
 * A183869  a(n) = n + floor(sqrt(4*n + 5)); complement of A004116.
 * https://oeis.org/A183869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(4*n + 5))
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute v = 4*n + 5 avoiding overflow for the range of n needed. */
    idx_t v = 4*n + 5;

    /* Integer square root of v using the shift‑and‑subtract method. */
    idx_t r = 0;
    idx_t bit = ((idx_t)1 << 62);          /* highest power of four <= 2^64 */
    while (bit > v) bit >>= 2;             /* find starting bit */

    idx_t remaining = v;
    while (bit) {
        if (remaining >= r + bit) {
            remaining -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }

    return (seq_t)n + (seq_t)r;
}

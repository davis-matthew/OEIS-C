/*
 * A076478  The binary Champernowne sequence: concatenate binary vectors of
 *          lengths 1, 2, 3, ... in numerical order.
 * https://oeis.org/A076478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Binary Champernowne sequence:
   a(n) is the n-th bit of the infinite concatenation of binary
   vectors obtained by writing each integer m ≥ 1 in binary,
   dropping the leading 1, and outputting the remaining bits
   in order.  Thus the sequence begins:
   0,1,0,0,0,1,1,0,1,1,0,0,0…   */

/* compute the bit-length of a 64‑bit unsigned integer */
static inline unsigned bitlen_idx(idx_t x)
{
    unsigned len = 0;
    while (x != 0) {
        x >>= 1;
        ++len;
    }
    return len;
}

seq_t a(idx_t n)
{
    idx_t remaining = n;

    for (idx_t m = 1ULL; ; ++m) {
        unsigned len = bitlen_idx(m) - 1;   /* number of bits produced by m */

        if (len == 0)          /* m = 1 contributes no bits */
            continue;

        if (remaining < len) { /* desired bit lies in this m */
            unsigned shift = len - 1 - (unsigned)remaining;
            return (seq_t)((m >> shift) & 1U);
        }

        remaining -= len;
    }
}

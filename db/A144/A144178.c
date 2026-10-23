/*
 * A144178  a(n) = b(2n-1)*b(2n) where b(3n+1) = floor(n/9) + 2, b(3n+2) = (n
 *          mod 9) + 2, b(3n+3) = b(3n+1)*b(3n+2) for n >= 0.
 * https://oeis.org/A144178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = b(2n-1) * b(2n) where b(3k+1) = floor(k/9) + 2 b(3k+2) =
 *            (k mod 9) + 2 b(3k+3) = b(3k+1) * b(3k+2), for k >= 0 The
 *            sequence is defined for n >= 1; a(1) = 4
 * Range:     u8 n <= 26, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t
b_of(idx_t x)
{
    idx_t k   = (x - 1) / 3;
    idx_t rem = (x - 1) % 3;

    if (rem == 0) {
        return (seq_t)(k / 9 + 2);
    } else if (rem == 1) {
        return (seq_t)((k % 9) + 2);
    } else {            /* rem == 2 */
        seq_t b1 = (seq_t)(k / 9 + 2);          /* b(3k+1) */
        seq_t b2 = (seq_t)((k % 9) + 2);         /* b(3k+2) */
        return b1 * b2;
    }
}

seq_t a(idx_t n)
{
    idx_t i1 = 2 * n - 1;   /* index for b(2n-1) */
    idx_t i2 = 2 * n;       /* index for b(2n)   */
    seq_t b1 = b_of(i1);
    seq_t b2 = b_of(i2);
    return b1 * b2;
}

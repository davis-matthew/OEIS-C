/*
 * A162155  Integer quartets a(4k)= 2, a(4k+1) = 32*k^2-24*k+3, a(4k+2) =
 *          32*k^2-24*k+2, a(4k+3) = 8*k-3, k>=1.
 * https://oeis.org/A162155
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(4k) = 2 a(4k+1) = 32*k^2 – 24*k + 3 a(4k+2) = 32*k^2 – 24*k +
 *            2 a(4k+3) = 8*k – 3 offset: 4 (k ≥ 1)
 * Range:     u8 n <= 16, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) return 0;          /* values before the offset are irrelevant */
    idx_t k = n >> 2;             /* floor(n/4) */

    switch (n & 3) {
        case 0: return (seq_t)2;
        case 1: return (seq_t)((seq_t)32 * k * k - (seq_t)24 * k + (seq_t)3);
        case 2: return (seq_t)((seq_t)32 * k * k - (seq_t)24 * k + (seq_t)2);
        case 3: return (seq_t)((seq_t)8 * k - (seq_t)3);
    }
    return 0; /* unreachable */
}

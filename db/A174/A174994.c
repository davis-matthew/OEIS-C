/*
 * A174994  Repeat (8*n+4)^2.
 * https://oeis.org/A174994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = 16*n^2 for odd n; a(n) = 16*n^2 + 32*n + 16 for even n
 * Range:     u8 n <= 3, u16 n <= 63, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1ULL) == 0) {             /* n even */
        return ((seq_t)16 * nn * nn) + ((seq_t)32 * nn) + ((seq_t)16);
    } else {                            /* n odd */
        return ((seq_t)16 * nn * nn);
    }
}

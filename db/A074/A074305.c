/*
 * A074305  a(3m) = 2m, a(3m+1) = 4m+3, a(3m+2) = 4m+1.
 * https://oeis.org/A074305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3m) = 2m, a(3m+1) = 4m+3, a(3m+2) = 4m+1
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n / 3;
    idx_t r = n % 3;
    switch (r) {
        case 0: return ((seq_t)m) * 2;
        case 1: return ((seq_t)m) * 4 + 3;
        case 2: return ((seq_t)m) * 4 + 1;
        default: return 0; /* unreachable */
    }
}

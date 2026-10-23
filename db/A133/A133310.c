/*
 * A133310  a(3n) = 2n+1, a(3n+1) = 2n+2, a(3n+2) = 2n+1.
 * https://oeis.org/A133310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3n) = 2n+1, a(3n+1) = 2n+2, a(3n+2) = 2n+1
 * Range:     u8 n <= 381, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = n / 3;          /* the integer n = 3q + r */
    idx_t r = n % 3;

    switch (r) {
    case 0: return ((seq_t)q) * 2 + 1;   /* a(3q)   */
    case 1: return ((seq_t)q) * 2 + 2;   /* a(3q+1) */
    case 2: return ((seq_t)q) * 2 + 1;   /* a(3q+2) */
    default: return 0; /* unreachable */
    }
}

/*
 * A058281  Continued fraction for square root of e.
 * https://oeis.org/A058281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, nonn, easy, nice
 * Formula:   a(n) = (n % 3 == 1) ? 4*((n-1)/3) + 1 : 1
 * Range:     u8 n <= 192, u16 n <= 8634, u32 n <= 8634, u64 n <= 8634,
 *            u128 n <= 8634, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Thus a(3k)   = 1, a(3k+1) = 4k+1, a(3k+2) = 1          */
seq_t a(idx_t n)
{
    if (n % 3 == 1) {
        seq_t k = (seq_t)(n - 1) / 3;
        return 4 * k + 1;
    }
    return 1;
}

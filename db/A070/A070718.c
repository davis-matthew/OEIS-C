/*
 * A070718  n^7 mod 37.
 * https://oeis.org/A070718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 mod 37
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce the index modulo 37 first to keep intermediate values tiny */
    idx_t m = n % 37;
    seq_t base = (seq_t)m;   /* base < 37 */
    seq_t result = (seq_t)1; /* current power */

    /* Compute base^7 modulo 37 iteratively */
    for (int i = 0; i < 7; ++i) {
        result = (result * base) % 37;
    }

    return result;
}

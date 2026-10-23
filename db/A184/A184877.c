/*
 * A184877  a(n) = n^2*(n-2)^2*(n-4)^2*...*(1 or 2)^2.
 * https://oeis.org/A184877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!!)^2; a(n) = n^2 * a(n-2), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* small indices */
    if (n < 2)
        return 1;

    seq_t result = 1;
    for (idx_t k = n; k >= 2; k -= 2)
    {
        /* (k^2) * result  – multiplication done in seq_t arithmetic */
        seq_t term = (seq_t)k * (seq_t)k;
        result *= term;
    }
    return result;
}

/*
 * A006474  Related to Ramsey numbers.
 * https://oeis.org/A006474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(s^2/4) with s = floor((5n-1)/3) for n >= 2, and
 *            a(1) = 1 (fitted to the listed terms; not checked against the
 *            b-file)
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t s, h;
    if (n == 1)
        return (seq_t)1;
    s = ((seq_t)n * 5 - 1) / 3;
    h = s / 2;
    return h * (s - h);
}

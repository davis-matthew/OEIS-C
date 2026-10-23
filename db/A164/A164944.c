/*
 * A164944  Decimal value of the concatenation of first n even numbers in
 *          binary.
 * https://oeis.org/A164944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = binary concatenation of the first n even numbers *
 *            interpreted as a binary number
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n == 0 the OEIS offset does not apply, return 0 for safety */
    if (n == 0)
        return 0;

    seq_t res = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t val = k << 1;            /* 2 * k, the k-th even number */
        /* compute the bit length of val */
        unsigned len = 0;
        idx_t tmp = val;
        while (tmp) {
            ++len;
            tmp >>= 1;
        }
        /* shift current result and append the bits of val */
        res = (res << len) | (seq_t)val;
    }
    return res;
}

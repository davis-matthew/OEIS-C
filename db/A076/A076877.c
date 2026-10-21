/*
 * A076877  a(n) = A020330(n) / n.
 * https://oeis.org/A076877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 2^(1 + floor(log2(n)))
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for n = 0; return 0 for safety */
        return 0;

    /* compute floor(log2(n)) : position of the highest set bit */
    idx_t pos = 0;
    idx_t tmp = n;
    while (tmp >>= 1)
        ++pos;          /* pos now equals floor(log2(n)) */

    /* exponent is 1 + floor(log2(n)) */
    idx_t exp = pos + 1;

    /* compute 2^exp safely in seq_t */
    seq_t pow2 = ((seq_t)1) << exp;

    return pow2 + 1;
}

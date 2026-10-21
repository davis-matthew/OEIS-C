/*
 * A059126  A hierarchical sequence (W2{2} according to the description in the
 *          attached file - see link).
 * https://oeis.org/A059126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(3k+1) = 2*(v2(k+1)+1), a(3k) = a(3k+1) - 1, a(3k+2) = a(3k+1)
 *            - 1 where v2(n) is the exponent of the highest power of two
 *            dividing n
 * Range:     u8 n <= 9301, u16 n <= 9301, u32 n <= 9301, u64 n <= 9301,
 *            u128 n <= 9301, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* k = floor(n/3) : the index of the 3‑term block   */
    idx_t k = n / 3;

    /* t = k + 1 : argument of v2                        */
    idx_t t = k + 1;

    /* compute v2(t): count trailing zeros of t          */
    idx_t v = 0;
    while ((t & 1) == 0) {
        v++;
        t >>= 1;
    }

    /* base value for a(3k+1) */
    seq_t base = 2 * (seq_t)(v + 1);

    /* adjust for the other two positions */
    return (n % 3 == 1) ? base : (base - 1);
}

/*
 * A030076  a(n) = 10 - m, where m = maximal digit of n.
 * https://oeis.org/A030076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10 - maxdigit(n)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle index 0 gracefully – it is outside the sequence's offset */
    if (n == 0)
        return (seq_t)10;  /* arbitrary, but unused in the sequence */

    idx_t tmp = n;
    unsigned int maxd = 0;
    while (tmp > 0)
    {
        unsigned int d = tmp % 10;
        if (d > maxd)
            maxd = d;
        tmp /= 10;
    }
    return (seq_t)(10 - maxd);
}

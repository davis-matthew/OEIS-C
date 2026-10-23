/*
 * A043284  Maximal run length in base-10 representation of n.
 * https://oeis.org/A043284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = maximum run length of equal decimal digits in n's decimal representation */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* n=0 has one digit “0” */

    idx_t tmp   = n;
    idx_t prev  = tmp % 10;               /* first digit */
    idx_t count = 1;
    idx_t maxlen = 1;
    tmp /= 10;

    while (tmp > 0)
    {
        idx_t cur = tmp % 10;
        if (cur == prev)
        {
            count++;
            if (count > maxlen) maxlen = count;
        }
        else
        {
            prev  = cur;
            count = 1;
        }
        tmp /= 10;
    }
    return (seq_t)maxlen;
}

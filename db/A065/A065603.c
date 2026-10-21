/*
 * A065603  Transposition diameter: maximal number of moves in an optimal
 *          sorting of n objects by moving blocks.
 * https://oeis.org/A065603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, more, hard
 * Formula:   a(n) = ceil((n+1)/2) for n >= 3; a(1)=0, a(2)=1; exceptions:
 *            a(13)=8, a(15)=9
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)                     /* a(1)=0, a(2)=1 */
        return (seq_t)(n - 1);

    /* ceil((n+1)/2)  ==  (n+2)/2  for integers */
    seq_t res = ((seq_t)n + 2) / 2;

    /* known exceptions */
    if (n == 13) return (seq_t)8;
    if (n == 15) return (seq_t)9;

    return res;
}

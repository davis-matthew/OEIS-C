/*
 * A038457  |First digit-last digit| for triangular numbers.
 * https://oeis.org/A038457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = |first_digit(T_n) - last_digit(T_n)| where T_n =
 *            n*(n+1)/2
 * Range:     u8 n <= 22, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the nth triangular number */
    seq_t t = (seq_t)n * ((seq_t)n + 1) / 2;

    /* last (least significant) decimal digit */
    seq_t last = t % (seq_t)10;

    /* most significant decimal digit */
    seq_t first = t;
    while (first > (seq_t)9) {
        first /= 10;
    }

    /* absolute difference between the two digits */
    int diff = (int)first - (int)last;
    if (diff < 0) diff = -diff;

    return (seq_t)diff;
}

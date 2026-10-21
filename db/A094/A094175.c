/*
 * A094175  Round( n / sum of digits of n ).
 * https://oeis.org/A094175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = round(n / sumdigit(n))
 * Range:     u8 n <= 975, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the sum of decimal digits of n */
    seq_t sum = 0;
    idx_t t = n;
    while (t > 0) {
        sum += (seq_t)(t % 10);
        t /= 10;
    }

    /* calculate the rounded quotient: floor((n + sum/2)/sum) */
    seq_t num   = (seq_t)n;
    seq_t half  = sum / 2;          /* integer division */
    seq_t result = (num + half) / sum;

    return result;
}

/*
 * A128201  Union of positive squares and the odd numbers.
 * https://oeis.org/A128201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth smallest integer that is either odd or a perfect
 *            square
 * Range:     u8 n <= 134, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t odd = 1;                       /* next odd number to consider */
    idx_t evencnt = 1;                   /* counter k: the kth even square (k=1 → 2, k=2 → 4, …) */
    seq_t even_square = 4;               /* (2*1)^2 */

    idx_t count = 0;
    seq_t result = 0;

    while (count < n)
    {
        if (odd <= even_square)
        {
            result = odd;
            odd += 2;
        }
        else
        {
            result = even_square;
            ++evencnt;                  /* move to next even square */
            seq_t two_m = (seq_t)2 * (seq_t)evencnt;
            even_square = two_m * two_m;
        }
        ++count;
    }
    return result;
}

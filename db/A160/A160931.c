/*
 * A160931  a(n)= n + digital sum(n+1).
 * https://oeis.org/A160931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + digital_sum(n+1)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;      /* convert index to seq_t */
    seq_t inc   = n_seq + 1;     /* n+1 as seq_t */
    seq_t sum   = 0;             /* running digital sum */

    /* compute digital sum of inc (decimal digits) */
    while (inc > 0)
    {
        sum += inc % 10;    /* add last digit */
        inc /= 10;          /* discard last digit */
    }

    return n_seq + sum;     /* result */
}

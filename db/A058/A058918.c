/*
 * A058918  Number of elements required for synthesis of rational function of
 *          degree n by modified Bott and Duffin method.
 * https://oeis.org/A058918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = ((n%2==0)?5:6) * 2^floor(n/2) - 5
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for the modified Bott and Duffin method */
seq_t a(idx_t n)
{
    /* floor(n/2) */
    idx_t k = n >> 1;
    seq_t power = (seq_t)1;         /* 2^k */
    for (idx_t i = 0; i < k; ++i)
        power <<= 1;                /* multiply by 2 each step */

    seq_t multiplier = (n & 1) ? (seq_t)6 : (seq_t)5;
    return multiplier * power - (seq_t)5;
}

/*
 * A085527  a(n) = (2n+1)^n.
 * https://oeis.org/A085527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert to seq_t first to avoid mixing types */
    seq_t base = (seq_t)2 * (seq_t)n + (seq_t)1;
    seq_t result = (seq_t)1;
    idx_t exp = n;

    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }

    return result;
}

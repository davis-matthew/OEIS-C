/*
 * A096138  a(1) = 1, a(n) = digit reversal of n*a(n-1).
 * https://oeis.org/A096138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 1, a(n) = reverse_digits( n * a(n-1) ) for n ≥ 2
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 14, u64 n <= 22, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) is the seed value */
    if (n == 0) return (seq_t)0;          /* safety, not used in OEIS offset */
    seq_t prev = (seq_t)1;                 /* a(1) */
    if (n == 1) return prev;

    /* iterate from 2 up to n */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t prod = (seq_t)i * prev;      /* n * a(n-1) */
        seq_t rev  = 0;
        while (prod > (seq_t)0)
        {
            rev = rev * (seq_t)10 + (prod % (seq_t)10);
            prod = prod / (seq_t)10;
        }
        prev = rev;                        /* a(n) */
    }

    return prev;
}

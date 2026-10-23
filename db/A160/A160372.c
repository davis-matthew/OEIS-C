/*
 * A160372  The 4-tuple (2, ((2*n+1)^2-1)/2, ((2*n+3)^2-1)/2, a(n)), where
 *          a(n) = 4*(3 + 20n + 42n^2 + 32n^3 + 8n^4), has Diophantus's
 *          property that the product of any two distinct terms plus one is a
 *          square.
 * https://oeis.org/A160372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 * (2*n + 1) * (2*n + 3) * (2*n*n + 4*n + 1)
 * Range:     u8 none, u16 n <= 5, u32 n <= 106, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; return 0 for n == 0 for safety. */
    if (n == 0)
        return 0;

    seq_t sn = (seq_t)n;        /* cast index to seq_t for multiplications   */
    seq_t t1 = sn * 2 + 1;      /* 2*n + 1                                  */
    seq_t t2 = sn * 2 + 3;      /* 2*n + 3                                  */
    seq_t t3 = sn * sn * 2 + sn * 4 + 1; /* 2*n^2 + 4*n + 1                 */

    return 4 * t1 * t2 * t3;
}

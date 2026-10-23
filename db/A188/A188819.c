/*
 * A188819  Number of n X 3 binary arrays without the pattern 0 1 diagonally
 *          or antidiagonally.
 * https://oeis.org/A188819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for odd n: a(n) = 4*n*(n+1); for even n: a(n) = (2*n+1)^2
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset is 1; for n < 1 behaviour is irrelevant. */
    seq_t N = (seq_t)n;
    if ((n & 1) == 0)      /* even n */
    {
        seq_t t = 2 * N + 1;
        return t * t;
    }
    else                   /* odd n */
    {
        return 4 * N * (N + 1);
    }
}

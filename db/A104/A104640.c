/*
 * A104640  Number of odd digits in n^3.
 * https://oeis.org/A104640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = number of odd digits in n^3
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 2375, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n^3 in seq_t type */
    seq_t cube = (seq_t)n;
    cube *= cube;
    cube *= (seq_t)n;

    /* count odd digits of cube */
    seq_t cnt = 0;
    seq_t x   = cube;
    while (x != 0)
    {
        seq_t d = x % 10;
        if (d & 1)   /* d is odd if lowest bit is 1 */
            ++cnt;
        x /= 10;
    }
    return cnt;
}

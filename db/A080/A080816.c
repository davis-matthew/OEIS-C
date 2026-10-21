/*
 * A080816  Triangle read by rows in which n-th row gives trajectory of n
 *          (omitting n itself) under the map k -> k+1 if k odd, k -> k/2 if k
 *          even.
 * https://oeis.org/A080816
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) is the nth term in the flattened triangle obtained by
 *            iterating the map k -> k+1 if k is odd, k -> k/2 if k is even,
 *            omitting the starting number in each row
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset is 2; the behaviour for n < 2 does not matter. */
    if (n < 2) return 0;

    idx_t counter = 2;                /* position of the next term to output */
    for (idx_t start = 2; ; ++start)  /* rows start at 2 */
    {
        seq_t t = (seq_t)start;       /* current value before mapping */
        for (;;)
        {
            t = (t & 1) ? t + 1 : t / 2;   /* apply the map */
            if (counter == n) return t;    /* found the desired term */
            ++counter;
            if (t == 1) break;             /* end of this row */
        }
    }
}

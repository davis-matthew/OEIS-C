/*
 * A128882  a(n) = n!! - 1.
 * https://oeis.org/A128882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!! - 1
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0ULL)          /* 0!! = 1, so a(0) = 1 - 1 = 0 */
        return 0;

    seq_t prod = 1;

    if (n % 2ULL == 0ULL)   /* even n: n!! = n*(n-2)*...*2 */
    {
        for (idx_t k = n; k >= 2; k -= 2)
            prod *= (seq_t)k;
    }
    else                    /* odd n: n!! = n*(n-2)*...*1 */
    {
        for (idx_t k = n; ; k -= 2)
        {
            prod *= (seq_t)k;
            if (k <= 1)        /* stop after multiplying by 1 */
                break;
        }
    }

    return prod - 1;        /* a(n) = n!! - 1 */
}

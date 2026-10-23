/*
 * A036832  Schoenheim bound L_1(n,5,4).
 * https://oeis.org/A036832
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n/5 * ceil((n-1)/4 * ceil((n-2)/3 * ceil((n-3)/2 *
 *            1)))
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5)          /* sequence undefined below the offset */
        return 0;

    const int r = 4;          /* r = 4 for the bound L₁(n,5,4) */
    seq_t val = 1;            /* start with L₁(n−r,5−r,0) = 1 */

    for (int i = r - 1; i >= 0; --i)
    {
        seq_t current_n = (seq_t)(n - i);
        seq_t current_k = (seq_t)(5 - i);   /* k = 5 */
        /* ceil(val * current_n / current_k) */
        val = (val * current_n + current_k - 1) / current_k;
    }
    return val;
}

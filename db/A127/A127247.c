/*
 * A127247  A Thue-Morse falling factorial triangle.
 * https://oeis.org/A127247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = product_{j=k+1}^{r} TM(j) where r = ⌊(√(8*n+1)-1)/2⌋, k =
 *            n - r*(r+1)/2, and TM(j) = 0 if popcount(j) is even, else 1. The
 *            function returns 1 if all TM(j) in the range are 1, otherwise 0
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index within the row */
    idx_t k = n - r * (r + 1) / 2;

    if (k == r)
        return (seq_t)1;          /* empty product – diagonal entry */

    /* Check whether any TM(j) is zero in the range [k+1, r] */
    for (idx_t j = k + 1; j <= r; ++j) {
        if ((__builtin_popcountll(j) & 1) == 0)
            return (seq_t)0;      /* zero found, product becomes zero */
    }
    return (seq_t)1;              /* all entries were one */
}

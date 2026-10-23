/*
 * A164832  Least nonnegative integer k such that the decimal representations
 *          of k and k+1 have n distinct digits in common.
 * https://oeis.org/A164832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, fini, full, nonn
 * Formula:   a(n) = Least nonnegative integer k such that k and k+1 share
 *            exactly n distinct decimal digits. The sequence is defined only
 *            for 0 ≤ n ≤ 10; for n > 10 no such integer exists
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n = 0 : 0 shares no digits with 1 */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* base cases n = 1..9 follow a regular pattern:
       k = 1 0 2 3 … (n‑1) 0
       which contains the distinct digits 0,1,…,(n‑1). */
    if (n == (idx_t)1)
        return (seq_t)10;

    if (n <= (idx_t)9) {
        seq_t k = (seq_t)1;
        k = k * (seq_t)10 + (seq_t)0;          /* "10"  */
        for (idx_t d = 2; d < n; ++d)            /* digits 2 … n‑1 */
            k = k * (seq_t)10 + (seq_t)d;
        k = k * (seq_t)10 + (seq_t)0;          /* trailing 0 */
        return k;
    }

    /* n = 10 requires all 10 digits to appear in common.  The minimal
       solution is */
    if (n == (idx_t)10) {
        seq_t k = (seq_t)1;
        k = k * (seq_t)10 + (seq_t)0;          /* "10" */
        for (idx_t d = 2; d <= (idx_t)8; ++d)    /* digits 2 … 8 */
            k = k * (seq_t)10 + (seq_t)d;
        k = k * (seq_t)10 + (seq_t)8;          /* extra 8 */
        k = k * (seq_t)10 + (seq_t)9;          /* final 9 */
        return k;
    }

    /* For n > 10 no such integer exists.  Return 0. */
    return (seq_t)0;
}

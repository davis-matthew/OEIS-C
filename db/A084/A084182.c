/*
 * A084182  a(n) = 3^n + (-1)^n - [1/(n+1)], where [] represents the floor
 *          function.
 * https://oeis.org/A084182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n + (-1)^n - ⌊1/(n+1)⌋
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute 3^n, add or subtract 1 depending on n's parity,
 *  and subtract 1 only when n == 0 (because ⌊1/(n+1)⌋ = 1 only for n = 0).
 */
seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;           /* start with 3^0 = 1 */

    /* multiply by 3 exactly n times – works for all valid seq_t sizes */
    for (idx_t i = 0; i < n; ++i)
        res *= (seq_t)3;

    /* add (-1)^n */
    if ((n & 1) == 0)
        res += (seq_t)1;   /* n even  → +1 */
    else
        res -= (seq_t)1;   /* n odd   → -1 */

    /* subtract ⌊1/(n+1)⌋ – which equals 1 only when n == 0 */
    if (n == 0)
        res -= (seq_t)1;

    return res;
}

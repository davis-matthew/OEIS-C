/*
 * A042126  Numerators of continued fraction convergents to sqrt(588).
 * https://oeis.org/A042126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 194*a(n-2) - a(n-4); initial terms: a(0)=24, a(1)=97,
 *            a(2)=4680, a(3)=18817. Recurrence holds for n >= 4
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    if (n == 0) return (seq_t)24;
    if (n == 1) return (seq_t)97;
    if (n == 2) return (seq_t)4680;
    if (n == 3) return (seq_t)18817;

    /* Even indices: use the even subsequence recurrence. */
    if ((n & 1) == 0) {
        idx_t k = n >> 1;           /* k = n/2 */
        seq_t prev2 = (seq_t)24;    /* e(0) */
        seq_t prev1 = (seq_t)4680;  /* e(1) */
        seq_t cur;
        for (idx_t i = 2; i <= k; ++i) {
            cur     = 194 * prev1 - prev2;
            prev2   = prev1;
            prev1   = cur;
        }
        return prev1;
    }

    /* Odd indices: use the odd subsequence recurrence. */
    idx_t k = (n - 1) >> 1;          /* k = (n-1)/2 */
    seq_t prev2 = (seq_t)97;         /* o(0) */
    seq_t prev1 = (seq_t)18817;      /* o(1) */
    seq_t cur;
    for (idx_t i = 2; i <= k; ++i) {
        cur     = 194 * prev1 - prev2;
        prev2   = prev1;
        prev1   = cur;
    }
    return prev1;
}

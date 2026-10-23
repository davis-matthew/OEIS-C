/*
 * A173712  Numbers n such that (n+n+1) divides the decimal concatenation [n,
 *          n+1].
 * https://oeis.org/A173712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A173712: Numbers n such that (n)+(n+1) divides the
   decimal concatenation [n , n+1].  The terms alternate between
   numbers of the form
        1 followed by ((n-1)/2) digits 6   (n odd)
   and
        4 followed by (n/2-1) digits 9      (n even)
   For example: 1, 4, 16, 49, 166, 499, …   */
seq_t a(idx_t n)
{
    if (n < 1U)           /* offset is 1; undefined for n=0 */
        return 0U;

    if ((n & 1U) == 1U) { /* odd n: 1 + ((n-1)/2) sixes */
        idx_t  k = n >> 1;          /* floor(n/2) = (n-1)/2            */
        seq_t  val = 1U;
        for (idx_t i = 0U; i < k; ++i)
            val = val * 10U + 6U;   /* append a 6 digit   */
        return val;
    } else {            /* even n: 4 + (n/2-1) nines          */
        idx_t  k = n >> 1;        /* n/2                            */
        seq_t  val = 4U;
        for (idx_t i = 0U; i + 1U < k; ++i) /* k-1 times append 9 */
            val = val * 10U + 9U;
        return val;
    }
}

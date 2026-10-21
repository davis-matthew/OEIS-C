/*
 * A203472  a(n) = Product_{3 <= i < j <= n+2} (i + j).
 * https://oeis.org/A203472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Prod_{3 <= i < j <= n+2} (i + j)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* undefined for n < 1 */

    seq_t result = (seq_t)1;        /* a(1) = 1 */

    /* For each new upper bound j = cur + 2 we multiply the new factors
       (i + j) with i ranging from 3 up to cur + 1. */
    for (idx_t cur = 2; cur <= n; ++cur) {
        idx_t j = cur + 2;
        for (idx_t i = (idx_t)3; i <= cur + 1; ++i) {
            seq_t term = (seq_t)(i + j);
            result *= term;
        }
    }

    return result;
}

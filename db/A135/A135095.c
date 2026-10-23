/*
 * A135095  a(1)=1, a(n) = a(n-1) + n^5 if n odd, a(n) = a(n-1) + n^2 if n is
 *          even.
 * https://oeis.org/A135095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + (n odd ? n^5 : n^2), a(1)=1
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 60, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t acc = 0;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t x = (seq_t)i;
        seq_t add;

        if (i & 1U) {               /* odd: n^5 */
            seq_t x2 = x * x;       /* n^2   */
            seq_t x4 = x2 * x2;     /* n^4   */
            add = x4 * x;           /* n^5   */
        } else {                    /* even: n^2 */
            add = x * x;
        }

        acc += add;
    }

    return acc;
}

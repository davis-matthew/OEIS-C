/*
 * A074394  a(n) = a(n-1)*a(n-2) - a(n-3) with a(1) = 1, a(2) = 2, and a(3) =
 *          3.
 * https://oeis.org/A074394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)*a(n-2) - a(n-3), a(1)=1, a(2)=2, a(3)=3
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases directly. */
    if (n == 0) {                 /* offset guard – undefined below 1 */
        return (seq_t)0;
    }
    if (n == 1) {                 /* a(1) = 1 */
        return (seq_t)1;
    }
    if (n == 2) {                 /* a(2) = 2 */
        return (seq_t)2;
    }
    if (n == 3) {                 /* a(3) = 3 */
        return (seq_t)3;
    }

    /* Iteratively compute the recurrence using only a few variables.
       The variables a1,a2,a3 hold successive terms a(i-3),a(i-2),a(i-1). */
    seq_t a1 = (seq_t)1;          /* a(1) */
    seq_t a2 = (seq_t)2;          /* a(2) */
    seq_t a3 = (seq_t)3;          /* a(3) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        next = a3 * a2;           /* a(i-1)*a(i-2)   */
        next = next - a1;         /* minus a(i-3)    */

        /* Shift the window for the next iteration. */
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;                    /* a(n) */
}

/*
 * A105787  a(1) = 1; a(m) = maximum numerator possible with a continued
 *          fraction [b(1);b(2),b(3),...b(m-1)], where
 *          (b(1),b(2),b(3),...b(m-1)) is a permutation of
 *          (a(1),a(2),a(3),...a(m-1)).
 * https://oeis.org/A105787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Maximal numerator of a continued fraction using the previous sequence values.
   Recurrence: a(1)=1, a(2)=1, a(3)=2, a(4)=5.
   For n≥5  a(n) = a(n-1)^2 + a(n-2)*a(n-3) + a(n-4). */
seq_t a(idx_t n)
{
    /* Handle the initial four terms directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;
    if (n == 4) return (seq_t)5;

    /* Variables holding the last four computed terms:
          old4 = a(n-4), old3 = a(n-3), old2 = a(n-2), old1 = a(n-1). */
    seq_t old4 = (seq_t)1;   /* a(1)   */
    seq_t old3 = (seq_t)1;   /* a(2)   */
    seq_t old2 = (seq_t)2;   /* a(3)   */
    seq_t old1 = (seq_t)5;   /* a(4)   */
    seq_t next;

    for (idx_t i = 5; i <= n; ++i) {
        /* a(i) = a(i-1)^2 + a(i-2)*a(i-3) + a(i-4) */
        next = old1 * old1 + old2 * old3 + old4;

        /* Shift the window for the next iteration. */
        old4 = old3;
        old3 = old2;
        old2 = old1;
        old1 = next;
    }
    return old1;
}

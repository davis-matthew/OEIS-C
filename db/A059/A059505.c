/*
 * A059505  Transform of A059502 applied to sequence 2,3,4,...
 * https://oeis.org/A059505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3) - a(n-4) Initial values:
 *            a(1)=2, a(2)=5, a(3)=14, a(4)=40 Valid for all n ≥ 5
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined below offset */

    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)14;
    if (n == 4) return (seq_t)40;

    seq_t p4 = (seq_t)2;          /* a(n-4) */
    seq_t p3 = (seq_t)5;          /* a(n-3) */
    seq_t p2 = (seq_t)14;         /* a(n-2) */
    seq_t p1 = (seq_t)40;         /* a(n-1) */
    seq_t next;

    for (idx_t i = 5; i <= n; ++i) {
        next = p1 * (seq_t)6
             - p2 * (seq_t)11
             + p3 * (seq_t)6
             - p4;
        p4 = p3;
        p3 = p2;
        p2 = p1;
        p1 = next;
    }

    return p1;
}

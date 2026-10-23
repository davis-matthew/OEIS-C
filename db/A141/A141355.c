/*
 * A141355  The Jacobsthal sequence, dropping each third term.
 * https://oeis.org/A141355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-2) + 8*a(n-4) Initial values: a(0)=1, a(1)=1,
 *            a(2)=5, a(3)=11
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)11;

    seq_t v0 = (seq_t)1; /* a(0)  */
    seq_t v1 = (seq_t)1; /* a(1)  */
    seq_t v2 = (seq_t)5; /* a(2)  */
    seq_t v3 = (seq_t)11;/* a(3)  */

    idx_t i;
    for (i = 4; i <= n; ++i) {
        /* compute a(i) = 7*a(i-2) + 8*a(i-4) */
        seq_t next = (seq_t)7 * v2 + (seq_t)8 * v0;

        /* shift the window: v0<-v1, v1<-v2, v2<-v3, v3<-next */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = next;
    }
    return v3;
}

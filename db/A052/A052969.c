/*
 * A052969  Expansion of (1-x)/(1-x-2x^2+x^4).
 * https://oeis.org/A052969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)=1, a(1)=0, a(2)=2, a(3)=2,
   a(n)=2*a(n-2)+a(n-1)-a(n-4)  (n>=4)
*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)2;

    seq_t v0 = (seq_t)1;   /* a(n-4) */
    seq_t v1 = (seq_t)0;   /* a(n-3) */
    seq_t v2 = (seq_t)2;   /* a(n-2) */
    seq_t v3 = (seq_t)2;   /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = v2 * 2 + v3 - v0;  /* a(n) */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = next;
    }
    return v3;
}

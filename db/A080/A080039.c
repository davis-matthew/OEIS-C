/*
 * A080039  a(n) = floor((1+sqrt(2))^n).
 * https://oeis.org/A080039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor((1+sqrt(2))^n) Computed via the Pell recurrence:
 *            P(0)=2, P(1)=2, P(k)=2*P(k-1)+P(k-2) a(n)=P(n) - ((n%2==0)?1:0)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;   /* a(0) = 1 */
    if (n == (idx_t)1) return (seq_t)2;   /* a(1) = 2 */

    /* compute Pell numbers up to P(n) */
    seq_t P0 = (seq_t)2; /* P(0) */
    seq_t P1 = (seq_t)2; /* P(1) */
    seq_t P;             /* temporary for P(k) */

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        P = P1 * (seq_t)2 + P0; /* P(k) = 2*P(k-1)+P(k-2) */
        P0 = P1;                /* shift window */
        P1 = P;
    }

    /* P1 now holds P(n) */
    if ((n & (idx_t)1) == (idx_t)0) {   /* even n */
        return P1 - (seq_t)1;
    } else {                             /* odd n */
        return P1;
    }
}

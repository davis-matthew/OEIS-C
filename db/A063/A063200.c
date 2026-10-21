/*
 * A063200  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          15 ).
 * https://oeis.org/A063200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A063200: dimension of the space of weight 2n cuspidal newforms for Γ0(15) */
/* Recurrence: a(n) = a(n-1) + a(n-3) - a(n-4) for n ≥ 6,
   with seed values a(1)=1, a(2)=2, a(3)=4, a(4)=4, a(5)=6 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)4;
    if (n == 5) return (seq_t)6;

    /* Sliding window for the last five terms */
    seq_t v0 = (seq_t)1;   /* a(1) */
    seq_t v1 = (seq_t)2;   /* a(2) */
    seq_t v2 = (seq_t)4;   /* a(3) */
    seq_t v3 = (seq_t)4;   /* a(4) */
    seq_t v4 = (seq_t)6;   /* a(5) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t newv = v4 + v2 - v1;  /* a(i) */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = v4;
        v4 = newv;
    }
    return v4;
}

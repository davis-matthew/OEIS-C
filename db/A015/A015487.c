/*
 * A015487  a(0)=1, a(1)=3, a(n) = sum_{k=0}^{k=n-1} 3^k a(k).
 * https://oeis.org/A015487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=10 , a(n) = sum_{k=0}^{n-1} 3^k a(k) (for
 *            n>=2) By exploiting the identity obvious for n>=3: a(n) = (1 +
 *            3^(n-1)) * a(n-1) the sequence can be built in O(n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)10;

    seq_t prev = (seq_t)10;          /* a(2) */
    seq_t pow3 = (seq_t)9;           /* 3^2, needed for a(3) */

    for (idx_t k = 3; k <= n; ++k) {
        seq_t factor = pow3 + (seq_t)1;   /* 1 + 3^(k-1) */
        seq_t curr   = prev * factor;     /* a(k) = a(k-1) * factor */
        prev = curr;
        pow3 *= (seq_t)3;                /* advance to 3^k for next loop */
    }
    return prev;
}

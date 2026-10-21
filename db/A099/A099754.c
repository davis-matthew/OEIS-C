/*
 * A099754  a(n) = (3^n +1)/2 + 2^n.
 * https://oeis.org/A099754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3^n + 1)/2 + 2^n
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)2;      /* a(0) = 2 */

    seq_t cur   = (seq_t)2;           /* a(0)                */
    seq_t pow3  = (seq_t)1;           /* 3^0                 */
    seq_t pow2  = (seq_t)1;           /* 2^0                 */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t next = cur + pow3 + pow2;   /* a(i) = a(i‑1) + 3^(i‑1) + 2^(i‑1) */
        if (i == n) return next;

        /* advance powers for the next iteration: 3^i , 2^i   */
        pow3 *= (seq_t)3;
        pow2 *= (seq_t)2;
        cur = next;
    }
    return cur;   /* should never reach here */
}

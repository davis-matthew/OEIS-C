/*
 * A085010  a(n)=2^(2^n)*sum(k=0,n,1/2^(2^k)).
 * https://oeis.org/A085010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = 1 + a(n) * 2^(2^n), a(0) = 1 (This recurrence follows
 *            from a(n) = 2^(2^n) * sum_{k=0}^n 1/2^(2^k).)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the offset */
    if (n == 0) return (seq_t)1;

    seq_t a_val = (seq_t)1;      /* a(0) */
    seq_t pow2   = (seq_t)2;     /* pow2 = 2^(2^0) */

    for (idx_t i = 0; i < n; ++i) {
        /* a(i+1) = 1 + a(i) * pow2 */
        a_val = a_val * pow2 + (seq_t)1;

        /* prepare pow2 for the next step: pow2 = pow2^2 = 2^(2^(i+1)) */
        pow2 = pow2 * pow2;
    }
    return a_val;
}

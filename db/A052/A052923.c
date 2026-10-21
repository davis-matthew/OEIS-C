/*
 * A052923  Expansion of (1-x)/(1 - x - 4*x^2).
 * https://oeis.org/A052923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-2), a(0)=1, a(1)=0
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) exactly for any seq_t type (+ arbitrary precision, + unsigned 8/16/32/64/128 bits).
   No dynamic memory is used, so the function works for all builds and for calls in any order. */
seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;          /* a(0) = 1 */
    if (n == 1u)
        return (seq_t)0;          /* a(1) = 0 */

    seq_t a_prev2 = (seq_t)1;     /* corresponds to a(n-2) */
    seq_t a_prev1 = (seq_t)0;     /* corresponds to a(n-1) */
    seq_t a_curr   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        a_curr = a_prev1 + (a_prev2 * (seq_t)4);
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}

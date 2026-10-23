/*
 * A025979  Expansion of 1/((1-2*x)*(1-4*x)*(1-8*x)*(1-12*x)).
 * https://oeis.org/A025979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4^(n+1)-2^(n+1))/2 + 20*a(n-1) - 96*a(n-2) Initial
 *            values: a(0) = 1, a(1) = 26
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)26;

    seq_t a_prev2 = (seq_t)1; /* a(n-2) */
    seq_t a_prev1 = (seq_t)26; /* a(n-1) */
    seq_t pow2i  = (seq_t)1;   /* 2^0 */

    for (idx_t i = 1; i <= n; ++i) {
        pow2i <<= 1;                      /* pow2i = 2^i   */
        if (i >= 2) {
            seq_t term = pow2i * ((pow2i << 1) - (seq_t)1); /* 2^(2i+1)-2^i */
            seq_t a_i   = term + a_prev1 * (seq_t)20 - a_prev2 * (seq_t)96;
            a_prev2 = a_prev1;
            a_prev1 = a_i;
        }
    }
    return a_prev1;
}

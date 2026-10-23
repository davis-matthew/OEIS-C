/*
 * A198017  a(n) = n*(7*n + 11)/2 + 1.
 * https://oeis.org/A198017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(7*n + 11)/2 + 1
 * Range:     u8 n <= 7, u16 n <= 136, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term = 7 * nn + (seq_t)11;   /* 7*n + 11 */
    if (nn & (seq_t)1)                  /* n odd → (7*n + 11) is even */
    {
        term = term / (seq_t)2;          /* divide the even part first */
        return nn * term + (seq_t)1;
    }
    else                                /* n even → n is divisible by 2 */
    {
        seq_t half = nn / (seq_t)2;
        return half * term + (seq_t)1;
    }
}

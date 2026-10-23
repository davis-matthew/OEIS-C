/*
 * A024206  Expansion of x^2*(1+x-x^2)/((1-x^2)*(1-x)^2).
 * https://oeis.org/A024206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = floor((n+1)^2/4) - 1
 * Range:     u8 n <= 14, u16 n <= 511, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0, a(2) = 1, a(3) = 3, ...  for n >= 1 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t before any arithmetic
       to avoid accidental promotion or overflow of idx_t */
    seq_t np1 = (seq_t)n + 1;      /* n + 1 */
    seq_t sq   = np1 * np1;        /* (n+1)^2 */
    seq_t res  = sq / 4;           /* floor division by 4 */
    res = res - 1;                 /* subtract 1 */
    return res;
}

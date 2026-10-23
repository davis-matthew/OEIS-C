/*
 * A017918  Expansion of 1/((1-3x)*(1-5x)*(1-12x)).
 * https://oeis.org/A017918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 20, a(2) = 289; for n > 2, a(n) = 20*a(n-1) -
 *            111*a(n-2) + 180*a(n-3)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)20;
    if (n == 2) return (seq_t)289;

    seq_t prev3 = 1;        /* a(n-3) */
    seq_t prev2 = 20;       /* a(n-2) */
    seq_t prev1 = 289;      /* a(n-1) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        /* Compute 20*prev1 - 111*prev2 + 180*prev3
           Addition before subtraction avoids unsigned underflow. */
        cur = (prev1 * (seq_t)20) + (prev3 * (seq_t)180) - (prev2 * (seq_t)111);
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}

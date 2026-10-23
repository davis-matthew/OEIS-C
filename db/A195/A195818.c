/*
 * A195818  Generalized 14-gonal numbers: m*(6*m-5), m =
 *          0,+1,-1,+2,-2,+3,-3,...
 * https://oeis.org/A195818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n*(n+1) + (2*n+1)*(-1)^n - 1)/2
 * Range:     u8 n <= 9, u16 n <= 207, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t n_s = (seq_t)n;

    /* Compute t = n*(n+1) */
    seq_t t = n_s * (n_s + 1);

    /* Start with 3 * t */
    seq_t res = (seq_t)3 * t;

    /* Add or subtract (2*n+1) based on parity of n */
    if ((n & 1ULL) == 0) {          /* n is even: (-1)^n = 1 */
        res += (seq_t)2 * n_s + 1;
    } else {                         /* n is odd: (-1)^n = -1 */
        res -= (seq_t)2 * n_s + 1;
    }

    /* Subtract 1 and divide by 2 */
    res -= 1;
    return res / 2;
}

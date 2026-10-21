/*
 * A052997  Expansion of (1+x-x^3)/((1-2*x)*(1-x^2)).
 * https://oeis.org/A052997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, for n>=2 if n is even: a(n)=2*a(n-1)+1 if n is
 *            odd: a(n)=2*a(n-1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    seq_t prev = (seq_t)3; /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        if ((i & 1) == 0) {          /* even i */
            prev = (prev << 1) + 1;  /* 2*a(i-1)+1 */
        } else {                     /* odd i */
            prev = (prev << 1);      /* 2*a(i-1)   */
        }
    }
    return prev;
}

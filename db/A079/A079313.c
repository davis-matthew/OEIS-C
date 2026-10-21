/*
 * A079313  a(n) is taken to be the smallest positive integer not already
 *          present which is consistent with the condition "n is a member of
 *          the sequence if and only if a(n) is odd".
 * https://oeis.org/A079313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1, a(2)=3, a(3)=5, a(4)=2; * For n≥5: * if n%4==0 :
 *            a(n)=6*(n/4) - 1 * if n%4==1 : a(n)=6*((n-1)/4) + 1 * if n%4==2
 *            : a(n)=n + 2 * if n%4==3 : a(n)=6*((n+1)/4) - 3 * These are
 *            derived from the OEIS entry A079313
 * Range:     u8 n <= 171, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the four special initial terms. */
    if (n == 0) return 0;          /* below offset, not used */
    if (n == 1) return 1;
    if (n == 2) return 3;
    if (n == 3) return 5;
    if (n == 4) return 2;

    /* Use the closed formulas for n ≥ 5. */
    switch (n & 3) {               /* n mod 4 */
        case 0: {                 /* n = 4t   */
            idx_t t = n >> 2;     /* t = n/4 */
            return (seq_t)6 * t - 1;
        }
        case 1: {                 /* n = 4t+1 */
            idx_t t = (n - 1) >> 2;
            return (seq_t)6 * t + 1;
        }
        case 2:                  /* n = 4t-2 */
            return (seq_t)n + 2;
        case 3: {                 /* n = 4t-1 */
            idx_t t = (n + 1) >> 2;
            return (seq_t)6 * t - 3;
        }
    }
    return 0;   /* unreachable, keep compiler happy */
}

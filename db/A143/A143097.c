/*
 * A143097  3*k - 2 interleaved with 3*k - 1 and 3*k.
 * https://oeis.org/A143097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(1)=1; for n>1, a(n) = (n-1) + (n-1) mod 3
 * Equivalent piecewise form:
 *   if (n%3==0) return n+1;
 *   if (n%3==1) return n-1;  // n>1
 *   if (n%3==2) return n;
 */
seq_t a(idx_t n)
{
    if (n == 0)     /* undefined behaviour for offset 1; safe guard */
        return 0;

    if (n == 1)
        return (seq_t)1;

    idx_t rem = n % 3;
    switch (rem) {
        case 0:                 /* n%3==0 -> a(n)=n+1 */
            return (seq_t)(n + 1);
        case 1:                 /* n%3==1 -> a(n)=n-1 */
            return (seq_t)(n - 1);
        case 2:                 /* n%3==2 -> a(n)=n   */
            return (seq_t)n;
    }
    /* unreachable, but keeps compiler happy */
    return 0;
}

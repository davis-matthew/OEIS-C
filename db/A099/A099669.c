/*
 * A099669  Partial sums of repdigits of A002276.
 * https://oeis.org/A099669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partial sums of repdigits of A002276:
 * a(1) = 2
 * a(n) = a(n-1) + rep(n), where rep(1)=2 and rep(k)=10*rep(k-1)+2 for k>1. */
seq_t a(idx_t n)
{
    /* Handle the trivial case n==1 or n==0 */
    if (n <= 1) {
        if (n == 0) return 0;   /* n=0 is in the domain of the function, not used by OEIS */
        return 2;
    }

    seq_t rep = 2;   /* repdigit with 1 digit */
    seq_t sum = 2;   /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        rep = rep * 10 + 2;   /* next repdigit */
        sum += rep;           /* add to partial sum */
    }

    return sum;
}

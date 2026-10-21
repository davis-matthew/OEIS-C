/*
 * A083661  G.f.: 1/(1-x) * Sum_{k>=0} x^2^(k+2)/(1+x^2^k).
 * https://oeis.org/A083661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=a(2)=a(3)=0; for n>3, if n is even then a(n)=a(n/2)+1, if n
 *            is odd then a(n)=a((n-1)/2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    /* Handle the initial values 1,2,3 */
    if (n <= 3) return 0;
    /* Repeatedly halve the index and count the number of even halves */
    while (n > 3) {
        if ((n & 1) == 0) {       /* n is even */
            res += 1;
        }
        n >>= 1;                  /* n = n / 2 */
    }
    return res;
}

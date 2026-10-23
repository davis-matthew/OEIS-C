/*
 * A182455  a(0)=1, a(n) = (a(n-1) mod (n+2))*(n+2).
 * https://oeis.org/A182455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(0)=1; for n>0: if n is even, a(n)=(n/2+1)*(n+4); if n is odd,
 *            a(n)=((n-1)/2+1)*(n+2)
 * Range:     u8 n <= 19, u16 n <= 359, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t nn = (seq_t)n;          /* cast index to seq_t for arithmetic */
    if ((nn & 1) == 0) {          /* even n */
        seq_t k = nn / 2;
        return (k + 1) * (nn + 4);
    } else {                      /* odd n */
        seq_t k = (nn - 1) / 2;
        return (k + 1) * (nn + 2);
    }
}

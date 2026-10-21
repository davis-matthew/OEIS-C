/*
 * A091830  a(1)=1; a(2n)=(a(n)+1) mod 2, a(2n+1)=a(2n)+1.
 * https://oeis.org/A091830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n==1)?1 : (n even? (a(n/2)+1) mod 2 : a(n-1)+1)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1)         /* a(1) = 1 */
        return 1;

    /* even index */
    if ((n & 1ULL) == 0) {
        seq_t v = a(n >> 1);          /* a(n/2)  */
        return (v + 1) & 1;           /* (a(n/2)+1) mod 2  */
    }

    /* odd index (n > 1) : n = 2m+1 => a(n) = a(2m)+1 */
    seq_t v = a(n - 1);               /* a(n-1) is a(2m) */
    return v + 1;                     /* a(2m)+1  */
}

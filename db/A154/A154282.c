/*
 * A154282  Dirichlet inverse of A154281.
 * https://oeis.org/A154282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n)=1 if n=1; a(n)=0 if n has an odd prime factor or n is 2^e
 *            with e odd; a(n)=(-1)^(e/2) if n=2^e with e even
 * Range:     i8 n <= 1500, i16 n <= 1500, i32 n <= 1500, i64 n <= 1500,
 *            i128 n <= 1500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    idx_t t = n;
    idx_t e = 0;
    /* count factors of 2 */
    while ((t & 1) == 0) {
        t >>= 1;
        e++;
    }
    /* odd factor present */
    if (t != 1) return (seq_t)0;
    /* exponent of 2 is odd -> zero */
    if (e & 1) return (seq_t)0;
    idx_t k = e >> 1; /* e/2 */
    return ((k & 1) == 0) ? (seq_t)1 : (seq_t)-1;
}

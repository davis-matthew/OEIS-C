/*
 * A040340  Continued fraction for sqrt(359).
 * https://oeis.org/A040340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative function: a(0)=18; for n>=1 let e=v₂(n),
   a(n)=1 if e==0, 17 if e==1, and 36 if e>=2. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)18;          /* explicit offset value */
    /* Count exponent of 2 in n */
    idx_t e = 0;
    idx_t t = n;
    while ((t & (idx_t)1) == 0) {
        ++e;
        t >>= 1;
    }
    if (e == 0) return (seq_t)1;           /* no factor 2 */
    if (e == 1) return (seq_t)17;          /* single power of 2 */
    return (seq_t)36;                      /* two or more powers of 2 */
}

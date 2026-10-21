/*
 * A060805  Numerators of special continued fraction for 2*zeta(3).
 * https://oeis.org/A060805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(1) = 2; a(2) = 1; for n ≥ 3 if n is odd: let k = (n + 1) / 2
 *            a(n) = (k/2) * ((k + 1)/2) + ((k + 1)/2) if n is even: let k = n
 *            / 2 a(n) = (k/2) * ((k + 1)/2)
 * Range:     u8 n <= 60, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined; safe return */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)1;

    if (n & (idx_t)1) {                /* odd n */
        seq_t k = ((seq_t)n + (seq_t)1) >> 1;          /* k = (n+1)/2 */
        seq_t t1 = k >> 1;                            /* t1 = k/2   */
        seq_t t2 = (k + (seq_t)1) >> 1;               /* t2 = (k+1)/2 = ceil(k/2) */
        return t1 * t2 + t2;                          /* floor(k^2/4) + ceil(k/2) */
    } else {                            /* even n */
        seq_t k = (seq_t)n >> 1;                          /* k = n/2   */
        seq_t t1 = k >> 1;                            /* t1 = k/2   */
        seq_t t2 = (k + (seq_t)1) >> 1;               /* t2 = (k+1)/2 */
        return t1 * t2;                               /* floor(k^2/4)   */
    }
}

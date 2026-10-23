/*
 * A033691  Minimal number of vertices in 1-1 deficient regular graph where
 *          minimal degree is 1 and maximal degree is n.
 * https://oeis.org/A033691
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   for n≥2 a(n) = n*(n/2+1) if n even 8* ((n+1)/4)^2 if n≡3 (mod 4)
 *            ((n-5)^2)/2 + 6*n - 10 if n≡1 (mod 4)
 * Range:     u8 n <= 20, u16 n <= 360, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)               /* values below the offset are irrelevant */
        return 0;

    /* Even n: a(n) = 2*i*(i+1) with i=n/2  →  n*(n/2+1) */
    if ((n & 1) == 0) {
        seq_t half = (seq_t)(n >> 1);
        return (seq_t)n * (half + 1);
    }

    /* n ≡ 3 (mod 4): a(n) = 8*((n+1)/4)^2 */
    if ((n % 4) == 3) {
        seq_t t = (seq_t)((n + 1) / 4);
        return (seq_t)8 * t * t;
    }

    /* n ≡ 1 (mod 4), n≥5: a(n) = ((n-5)^2)/2 + 6*n - 10 */
    seq_t t  = (seq_t)(n - 5);   /* even */
    seq_t t2 = t * t;
    return (t2 / 2) + (seq_t)6 * n - 10;
}

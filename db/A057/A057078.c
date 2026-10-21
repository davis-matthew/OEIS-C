/*
 * A057078  Periodic sequence 1,0,-1,...; expansion of (1+x)/(1+x+x^2).
 * https://oeis.org/A057078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 1 for n≡0 (mod 3), 0 for n≡1 (mod 3), –1 for n≡2 (mod 3)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n mod 3 */
    idx_t r = n % 3;
    if (r == 0u) {
        return (seq_t)1;
    } else if (r == 1u) {
        return (seq_t)0;
    } else { /* r == 2 */
        return (seq_t)(-1);
    }
}

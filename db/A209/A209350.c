/*
 * A209350  Number of initially rising meander words, where each letter of the
 *          cyclic n-ary alphabet occurs twice.
 * https://oeis.org/A209350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(0) = 1 a(1) = 0 a(2) = 1 for n ≥ 3: if n is even: a(n) = (n/2
 *            + 1)² else : a(n) = ((n-1)/2)² + 3·((n-1)/2) + 1
 * Range:     u8 n <= 29, u16 n <= 509, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    if ((n & (idx_t)1) == 0) {             /* even n ≥ 4 */
        seq_t k = (seq_t)(n / 2) + 1;       /* (n/2 + 1) */
        return k * k;                       /* (k)^2 */
    } else {                                /* odd n ≥ 3 */
        seq_t k = (seq_t)((n - 1) / 2);     /* (n-1)/2 */
        return k * k + 3 * k + 1;           /* k² + 3k + 1 */
    }
}

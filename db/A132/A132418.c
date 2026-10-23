/*
 * A132418  Sequence is identical to its third differences in absolute values:
 *          a(n+k)=3a(n+k-1)-3a(n+k-2)+2a(n+k-3), k=0, 1, 2, 3, 4,
 *          a(n+5)=3a(n+4)-3a(n+3), n > 2.
 * https://oeis.org/A132418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a[n mod 6] * 43^{floor(n/6)}, initial values: a(0)=1,
 *            a(1)=2, a(2)=3, a(3)=5, a(4)=10, a(5)=21
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[6] = {
        (seq_t)1, (seq_t)2, (seq_t)3,
        (seq_t)5, (seq_t)10, (seq_t)21
    };

    idx_t r = n % (idx_t)6;          /* remainder modulo 6   */
    idx_t m = n / (idx_t)6;          /* number of 6-blocks   */
    seq_t result = init[r];          /* base term            */

    /* compute 43^m   (simple loop, fast enough for n ≤ 10⁴) */
    seq_t pow = (seq_t)1;
    for (idx_t i = (idx_t)0; i < m; ++i)
        pow *= (seq_t)43;

    result *= pow;
    return result;
}

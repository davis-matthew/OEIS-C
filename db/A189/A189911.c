/*
 * A189911  Row sums of the extended Catalan triangle A189231.
 * https://oeis.org/A189911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = a(n-1)*2 if n is even; a(n) =
 *            a(n-1)*((⌊n/2⌋+2)*(2*⌊n/2⌋+1))/(⌊n/2⌋+1)² if n is odd
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 62, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;   /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        if ((i & 1) == 0)          /* even i */
        {
            res = res * 2;
        }
        else                       /* odd i */
        {
            idx_t k = i >> 1;      /* floor(i/2) */
            seq_t den = (seq_t)(k + 1);
            den = den * den;       /* (k+1)^2 */
            res = res / den;       /* safe division: divisible by den */
            seq_t num = (seq_t)(k + 2);
            num = num * (seq_t)(2 * k + 1);
            res = res * num;
        }
    }
    return res;
}

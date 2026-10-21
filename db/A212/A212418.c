/*
 * A212418  Size of the equivalence class of S_n containing the identity
 *          permutation under transformations of positionally adjacent
 *          elements of the form abc <--> acb <--> cba where a<b<c.
 * https://oeis.org/A212418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n < 3 a(2k+1) = (3/2) * k * (k+1) * (2k-1)! a(2k) =
 *            (3/2) * k * (k - 1/3) * (2k-2)! - (2k-3)!!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)1;

    if (n & 1) {               /* n is odd: n = 2k+1 */
        idx_t k = (n - 1) >> 1;
        seq_t fact = (seq_t)1;
        idx_t up = 2 * k - 1;
        for (idx_t i = 2; i <= up; ++i)
            fact *= (seq_t)i;

        seq_t mult = (seq_t)3 * (seq_t)k * (seq_t)(k + 1);
        return fact * mult / (seq_t)2;
    } else {                   /* n is even: n = 2k */
        idx_t k = n >> 1;
        if (k < 2) return (seq_t)1;   /* n = 2 */

        seq_t fact = (seq_t)1;
        idx_t up = 2 * k - 2;
        for (idx_t i = 2; i <= up; ++i)
            fact *= (seq_t)i;

        seq_t term = fact * (seq_t)k * (seq_t)(3 * k - 1) / (seq_t)2;

        seq_t dbl = (seq_t)1;
        for (idx_t i = 1; i <= 2 * k - 3; i += 2)
            dbl *= (seq_t)i;

        return term - dbl;
    }
}

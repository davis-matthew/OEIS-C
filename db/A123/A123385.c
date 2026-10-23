/*
 * A123385  a(n) = (n!)^2/2.
 * https://oeis.org/A123385
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^2/2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                 /* undefined below the offset */

    seq_t fact = 1;                /* n!   (n >= 2) */
    for (idx_t k = 2; k <= n; ++k)
        fact = fact * (seq_t)k;    /* multiply by k */

    seq_t result = fact * fact;    /* (n!)^2 */
    result = result / 2;           /* divide by 2 – integer division is exact */
    return result;
}

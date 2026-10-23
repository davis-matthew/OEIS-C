/*
 * A026216  Duplicate of A026177.
 * https://oeis.org/A026216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   * a(n) = (2n+1)/3 if n % 3 == 1 * a(n) = 2n if n % 3 == 2 * a(n)
 *            = (2n)/3 or 2n if n % 3 == 0 * For the last case, let k = n/3
 *            and repeatedly divide k by 3 * until k % 3 != 0. If k % 3 == 1
 *            then a(n) = (2n)/3, * otherwise (k % 3 == 2) a(n) = 2n
 * Range:     i8 n <= 61, i16 n <= 61, i32 n <= 61, i64 n <= 61, i128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t mod3 = n % 3;
    seq_t sn = (seq_t)n;

    if (mod3 == 1)
        return (sn * 2 + 1) / 3;
    if (mod3 == 2)
        return sn * 2;

    /* n is divisible by 3 */
    idx_t k = n / 3;
    while ((k % 3) == 0)
        k /= 3;

    if (k % 3 == 1)
        return (sn * 2) / 3;   /* 2n/3 */
    /* k % 3 == 2 */
    return sn * 2;              /* 2n */
}

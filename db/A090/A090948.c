/*
 * A090948  a(n) = (1/24)*(n+1)*(n+6)*(n^3+26*n^2+225*n+636).
 * https://oeis.org/A090948
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/24)*(n+1)*(n+6)*(n^3+26*n^2+225*n+636)
 * Range:     u8 n <= -1, u16 n <= 11, u32 n <= 152, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(-1) = 0 */
    if (n == -1)
        return 0;

    /* Cast the index to the sequence type */
    seq_t sn = (seq_t)n;

    /* The three factors of the numerator */
    seq_t f1 = sn + 1;                           /* n+1 */
    seq_t f2 = sn + 6;                           /* n+6 */
    seq_t f3 = sn * sn * sn                      /* n^3    */
        + 26 * sn * sn                           /* 26*n^2 */
        + 225 * sn                               /* 225*n  */
        + 636;                                   /* +636   */

    /* Divide the product by 24 = 3 * 2^3.
       We distribute the divisions across the three factors
       to keep the intermediate values within range. */

    /* First divide by 3 */
    if (f1 % 3 == 0)
        f1 /= 3;
    else if (f2 % 3 == 0)
        f2 /= 3;
    else
        f3 /= 3;

    /* Then divide by 8 (2^3) */
    int need2 = 3;
    while (need2 > 0) {
        if (f1 % 2 == 0) {
            f1 /= 2;
            need2--;
        } else if (f2 % 2 == 0) {
            f2 /= 2;
            need2--;
        } else {
            f3 /= 2;
            need2--;
        }
    }

    /* Finally multiply the divided factors */
    seq_t result = f1 * f2;
    result *= f3;
    return result;
}

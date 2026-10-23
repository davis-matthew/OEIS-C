/*
 * A116762  Number of permutations of length n which avoid the patterns 1234,
 *          1243, 3421.
 * https://oeis.org/A116762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 + 9*n^5 - 65*n^4 - 85*n^3 + 1624*n^2 - 3404*n +
 *            1440)/240 (n >= 4) a(1)=1, a(2)=2, a(3)=6
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not defined, return 0 */
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 6;

    /* compute using 128‑bit integer to avoid overflow for large n */
    __int128 nn = (__int128) n;
    __int128 num = nn * nn * nn * nn * nn * nn;          /* n^6 */
    num += 9 * nn * nn * nn * nn * nn;                  /* +9 n^5 */
    num -= 65 * nn * nn * nn * nn;                      /* -65 n^4 */
    num -= 85 * nn * nn * nn;                           /* -85 n^3 */
    num += 1624 * nn * nn;                              /* +1624 n^2 */
    num -= 3404 * nn;                                   /* -3404 n */
    num += 1440;                                        /* +1440 */

    return (seq_t)(num / 240);
}

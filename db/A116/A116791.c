/*
 * A116791  Number of permutations of length n which avoid the patterns 1234,
 *          1342, 4312.
 * https://oeis.org/A116791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^7 + 35*n^6 - 203*n^5 + 665*n^4 - 686*n^3 + 1820*n^2 -
 *            1632*n + 5040)/5040
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset: a(1) is first term. Return 0 for n==0, though it should not be called. */
    if (n == 0U)
        return 0;

    /* Use 128‑bit arithmetic for intermediate calculations to avoid overflow on 64‑bit builds. */
    __int128 x   = n;   /* promote index to 128‑bit */
    __int128 num = 1;   /* start with leading coefficient */

    num = num * x + 35;    /* add coefficient of n^6 */
    num = num * x - 203;   /* add coefficient of n^5 */
    num = num * x + 665;   /* add coefficient of n^4 */
    num = num * x - 686;   /* add coefficient of n^3 */
    num = num * x + 1820;  /* add coefficient of n^2 */
    num = num * x - 1632;  /* add coefficient of n^1 */
    num = num * x + 5040;  /* add constant term */

    /* Divide by 5040 (7!); the result is an integer by definition of the sequence. */
    num = num / 5040;

    return (seq_t)num;
}

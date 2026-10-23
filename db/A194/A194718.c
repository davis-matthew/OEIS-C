/*
 * A194718  Number of n-ary words beginning with the first character of the
 *          alphabet, that can be built by inserting six doublets into the
 *          initially empty word.
 * https://oeis.org/A194718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 10*(n-1) + 44*(n-1)^2 + 110*(n-1)^3 + 165*(n-1)^4 +
 *            132*(n-1)^5
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 32, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    seq_t k = (seq_t)n - 1;        /* k = n-1  (n>0, so no underflow) */
    seq_t result = 1;
    seq_t powk   = k;              /* k^1 */
    result += (seq_t)10 * powk;    /* 10*k */

    powk *= k;                     /* k^2 */
    result += (seq_t)44 * powk;    /* 44*k^2 */

    powk *= k;                     /* k^3 */
    result += (seq_t)110 * powk;   /* 110*k^3 */

    powk *= k;                     /* k^4 */
    result += (seq_t)165 * powk;   /* 165*k^4 */

    powk *= k;                     /* k^5 */
    result += (seq_t)132 * powk;   /* 132*k^5 */

    return result;
}

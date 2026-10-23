/*
 * A029039  Expansion of 1/((1-x)*(1-x^3)*(1-x^5)*(1-x^6)).
 * https://oeis.org/A029039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3 + 45*n^2 + 282*n + 60*n*[(n mod 3)=0] +
 *            1080)/1080) + [(n mod 30)=6]
 * Range:     u8 none, u16 n <= 24, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                 /* convert index to seq_t base type */
    seq_t n2 = nn * nn;                  /* n^2 */
    seq_t n3 = n2 * nn;                  /* n^3 */

    /* numerator = 2*n^3 + 45*n^2 + 282*n + 60*n*[(n mod 3)=0] + 1080 */
    seq_t numerator = 2 * n3 + 45 * n2 + 282 * nn;

    if (n % 3 == 0) {
        numerator += 60 * nn;
    }

    numerator += 1080;   /* constant offset */

    seq_t result = numerator / 1080;   /* integer division is floor for positive nums */

    /* add the extra 1 when n mod 30 == 6 */
    if (n % 30 == 6) {
        result += 1;
    }

    return result;
}

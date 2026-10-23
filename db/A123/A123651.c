/*
 * A123651  a(n) = 1 + n^2 + n^3 + n^5 + n^7 + n^11 + n^13 + n^17.
 * https://oeis.org/A123651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + n^2 + n^3 + n^5 + n^7 + n^11 + n^13 + n^17
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 13, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Small helper to compute base^exp for small exponents. */
static inline seq_t pow_seq(seq_t base, int exp)
{
    seq_t result = 1;
    for (int i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t res  = 1;                     /* the leading 1 */

    res += pow_seq(nseq, 2);            /* n^2  */
    res += pow_seq(nseq, 3);            /* n^3  */
    res += pow_seq(nseq, 5);            /* n^5  */
    res += pow_seq(nseq, 7);            /* n^7  */
    res += pow_seq(nseq, 11);           /* n^11 */
    res += pow_seq(nseq, 13);           /* n^13 */
    res += pow_seq(nseq, 17);           /* n^17 */

    return res;
}

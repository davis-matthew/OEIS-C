/*
 * A079909  Solution to the Dancing School Problem with 4 girls and n+4 boys:
 *          f(4,n).
 * https://oeis.org/A079909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 (n=0), 5 (n=1), n^4 – 2n^3 + 9n^2 – 8n + 6 (n>=2)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;
    if (n == 1u)
        return (seq_t)5;

    seq_t nsq = (seq_t)n * (seq_t)n;      /* n^2 */
    seq_t ncu = nsq * (seq_t)n;           /* n^3 */
    seq_t nqu = ncu * (seq_t)n;           /* n^4 */

    /* Compute n^4 – 2n^3 + 9n^2 – 8n + 6 */
    return nqu - ((seq_t)2 * ncu) + ((seq_t)9 * nsq) - ((seq_t)8 * (seq_t)n) + (seq_t)6;
}

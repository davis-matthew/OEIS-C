/*
 * A184541  Number of (n+2) X 4 binary arrays with each 3 X 3 subblock having
 *          rows and columns in lexicographically nondecreasing order.
 * https://oeis.org/A184541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 18 n^3 + 383 n^2 + 1206 n + 528) / 24
 * Range:     u8 none, u16 n <= 9, u32 n <= 200, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        abort();                    /* undefined for n < 1 */

    seq_t nn = (seq_t)n;            /* promote index to seq_t */
    seq_t num =
        nn * nn * nn * nn            /* n^4                         */
      + (seq_t)18 * nn * nn * nn     /* +18 n^3                     */
      + (seq_t)383 * nn * nn         /* +383 n^2                    */
      + (seq_t)1206 * nn            /* +1206 n                     */
      + (seq_t)528;                 /* +528                        */

    return num / (seq_t)24;         /* exact division */
}

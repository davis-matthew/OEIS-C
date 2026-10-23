/*
 * A178391  a(n) = (n^3+1)^3+1.
 * https://oeis.org/A178391
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3 + 1)^3 + 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The OEIS offset is -1, so a(-1) = 1 */
    if (n == -1)
        return (seq_t)1;

    if (n < -1)
        return 0;   /* undefined for n < -1, not required by the sequence */

    /* For n >= 0 compute (n^3 + 1)^3 + 1.
       We cast to seq_t only after ensuring 'n' is non‑negative. */
    seq_t x   = (seq_t)n;          /* n as unsigned */
    seq_t n3  = x * x * x;        /* n^3 (fits in seq_t for n up to the limit of the type) */
    seq_t t   = n3 + 1;           /* n^3 + 1 */
    seq_t u   = t * t * t;        /* (n^3 + 1)^3 */
    return u + 1;                 /* add the final +1 */
}

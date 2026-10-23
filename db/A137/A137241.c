/*
 * A137241  Number triples (k,3-k,2-2k), concatenated for k=0, 1, 2, 3,...
 * https://oeis.org/A137241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, less
 * Formula:   not stated
 * Range:     i8 n <= 199, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A137241: Number triples (k,3-k,2-2k), concatenated for k=0,1,2,3,...
   a(3k)   = k
   a(3k+1) = 3-k
   a(3k+2) = 2-2k
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t t = n/3;          /* k */
    unsigned r = n%3;       /* position in the triple */

    seq_t k = (seq_t)t;     /* convert k to seq_t for arithmetic */

    switch (r) {
    case 0: return k;                       /* a(3k)   = k          */
    case 1: return (seq_t)3 - k;            /* a(3k+1) = 3 - k       */
    default: return (seq_t)2 - (k << 1);    /* a(3k+2) = 2 - 2k      */
    }
}

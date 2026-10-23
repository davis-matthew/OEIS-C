/*
 * A101803  Nearest integer to n*(phi-1), where phi is golden ratio
 *          1.618033988749895... (A001622).
 * https://oeis.org/A101803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n*(phi-1)) = floor((floor(n*sqrt5) - n + 1)/2)
 * Range:     u8 n <= 7, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                  /* lift index to seq_t */
    seq_t tmp = (seq_t)5 * nn * nn;       /* 5*n*n  -> (n*sqrt5)^2  */
    seq_t s   = isqrt_seq(tmp);           /* floor(n*sqrt5)      */
    seq_t v   = (s - nn + (seq_t)1) / (seq_t)2; /* round(n*(phi-1)) */
    return v;
}

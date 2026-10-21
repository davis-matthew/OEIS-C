/*
 * A060202  Let G = complete graph on 4 vertices, create the sequence G, L(G),
 *          L(L(G)), L(L(L(G))), ... where each graph in this sequence is the
 *          line graph of the previous graph; a(n) is number of vertices of
 *          the n-th graph in this sequence.
 * https://oeis.org/A060202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * Π_{k=1..n} (1+2^{k-2})
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=4, for n>=1  a(n)=a(n-1)*(1+2^{n-2})      */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)4;

    seq_t result = (seq_t)4;
    /* Handle the term for k = 1 : (1+2^{-1}) = 3/2 */
    result = result * 3 / 2;

    if (n == 1)
        return result;

    seq_t power = (seq_t)1; /* holds 2^{k-2} for k=2 initially 2^0 = 1 */
    for (idx_t k = 2; k <= n; ++k) {
        result *= (power + (seq_t)1); /* multiply by (1 + 2^{k-2}) */
        power <<= 1;                  /* prepare 2^{(k+1)-2} for next round */
    }

    return result;
}

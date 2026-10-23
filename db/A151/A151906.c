/*
 * A151906  a(0) = 0, a(1) = 1; for n>1, a(n) = 8*A151905(n) + 4.
 * https://oeis.org/A151906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   For t ≥ 1 a(3t) = a(3t+1) = 4 * 3^(popcount(t)-1) a(3t+2) = 4 *
 *            3^(popcount(t)) Initial values: a(0)=0, a(1)=1, a(2)=4
 * Range:     u8 n <= 46, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly. */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 4;

    /* For n ≥ 3: write n = 3t + r (r = 0,1,2). */
    idx_t t = n / 3;
    unsigned r = (unsigned)(n % 3);

    /* Population count of t (t ≥ 1 here). */
    unsigned w = __builtin_popcountll((uint64_t)t);

    if (r == 2)
    {
        /* a(3t+2) = 4 * 3^w */
        return (seq_t)4 * pow3((idx_t)w);
    }
    else
    {
        /* r == 0 or 1: a(3t) = a(3t+1) = 4 * 3^(w-1) */
        return (seq_t)4 * pow3((idx_t)(w - 1));
    }
}

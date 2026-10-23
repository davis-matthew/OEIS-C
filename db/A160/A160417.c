/*
 * A160417  a(n) = A160415(n+1)/4.
 * https://oeis.org/A160417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2 For n > 1: let pc = popcount(n) // number of 1‑bits in
 *            the binary representation of n if (n is even) a(n) = 3^(pc‑1) if
 *            (n is odd) a(n) = 7 · 3^(pc‑2) This is equivalent to a(n) =
 *            A160415(n+1)/4
 * Range:     u8 n <= 62, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 3^e as a seq_t.  e is a small non‑negative integer (<=63). */
static seq_t pow3(uint64_t e)
{
    seq_t r = (seq_t)1;
    for (uint64_t i = 0; i < e; ++i)
        r *= (seq_t)3;
    return r;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)2;          /* special case, because pc‑2 would be negative */

    const uint64_t u = (uint64_t)n;            /* n fits in 64 bits for popcount */
    const int pc   = __builtin_popcountll(u); /* popcount of n, pc >= 1 */

    seq_t val;
    if ((u & 1ULL) == 0) {                /* n is even */
        val = pow3((uint64_t)(pc - 1));
    } else {                              /* n is odd and n > 1 */
        val = pow3((uint64_t)(pc - 2));
        val *= (seq_t)7;
    }
    return val;
}

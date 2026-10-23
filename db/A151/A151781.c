/*
 * A151781  Partial sums of A151779.
 * https://oeis.org/A151781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 85, u32 n <= 6137, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A151781: Partial sums of A151779.  
   Definition: a(1) = 1; for n > 1  
   a(n) = 1 + Σ_{k=2}^{n} 6 * 5^{popcount(k-1)-1}   
   where popcount(x) is the number of 1‑bits in the 64‑bit binary
   representation of x. */
#include <stdint.h>

static unsigned popcount_uint64(uint64_t x)
{
    unsigned c = 0;
    while (x) { x &= x - 1; ++c; }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 0, return 0 */
    seq_t result = 1;          /* a(1) */
    if (n == 1) return result;

    for (idx_t k = 2; k <= n; ++k)
    {
        uint64_t num = k - 1;
        unsigned pc = popcount_uint64(num);        /* popcount(k-1) */
        /* popcount is always ≥ 1 for k ≥ 2, so pc-1 ≥ 0 */
        seq_t pow5 = 1;
        unsigned exp = pc ? pc - 1 : 0;
        while (exp--) pow5 *= (seq_t)5;          /* 5^(pc-1) */
        seq_t inc = pow5 * (seq_t)6;             /* 6 * 5^(pc-1) */
        result += inc;
    }

    return result;
}

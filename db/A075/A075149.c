/*
 * A075149  Sum_{i=0..2*A053645(n)} (C(2*A053645(n),i) mod 2)*A000045(n-i)
 *          [where C(r,c) is the binomial coefficient (A007318) and A000045(n)
 *          is the n-th Fibonacci number].
 * https://oeis.org/A075149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0}^{2·A053645(n)} (C(2·A053645(n),i) mod 2) * F(n-i)
 *            where F(k) is the k‑th Fibonacci number (A000045)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/fib.h"

/* Return ⌊log₂(n)⌋ for n > 0.  For n = 0 the value is unused,
   so the caller must guard against it. */
static inline unsigned floor_log2_u64(idx_t n)
{
#if defined(__GNUC__) || defined(__clang__)
    /* __builtin_clzll undefined for 0, but caller ensures n>0   */
    return 63 - __builtin_clzll((unsigned long long)n);
#else
    unsigned k = 0;
    while (n >>= 1) ++k;
    return k;
#endif
}

/* A053645(n) = n – 2^{⌊log₂n⌋} for n ≥ 1.  For n = 0 the function
   is never called; the main routine handles n = 0 separately. */
static inline idx_t a053645(idx_t n)
{
    unsigned k = floor_log2_u64(n);
    idx_t msb = ((idx_t)1) << k;
    return n - msb;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* a(0) = 0 */

    /* Compute r = A053645(n).  Then 2·r = 2 * (n – 2^⌊log₂n⌋).  */
    idx_t r   = a053645(n);
    idx_t two_r = r << 1;                       /* 2 * r */

    seq_t sum = (seq_t)0;

    /* Iterate over all i such that 0 ≤ i ≤ two_r and
       C(two_r,i) ≡ 1 (mod 2).  For binary Lucas theorem
       this happens iff every set bit of i is also set in two_r.
       Because two_r = r << 1, every such i is of the form 2*s
       where s is a subset of the bits of r.  We iterate over
       all subsets s ⊆ bits(r).                                 */
    idx_t s = r;
    for (;;)
    {
        idx_t i = s << 1;            /* i = 2·s ⊆ two_r */
        seq_t f = fib(n - i);        /* Fibonacci number F(n-i) */
        sum += f;

        if (s == 0) break;           /* all subsets processed */
        s = (s - 1) & r;             /* next subset of r */
    }

    return sum;
}

/*
 * A108448  Number of peaks of the form ud in all paths from (0,0) to (3n,0)
 *          that stay in the first quadrant (but may touch the horizontal
 *          axis), consisting of steps u=(2,1), U=(1,2), or d=(1,-1).
 * https://oeis.org/A108448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(2*n-1,k)*binomial(n-1,k)*2^k
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#ifdef __cplusplus
#include <type_traits>
#endif

/* Computes the n-th term of OEIS sequence A108448.
   * n >= 1. For n == 0 the function returns 0 (offset is 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1; not used in usual calls */
    seq_t sum   = 0;
    seq_t term  = 1;               /* term for k = 0 */
    sum += term;

    for (idx_t k = 0; k + 1 < n; ++k)
    {
        /* Compute the next term in the sum using the recurrence
           term_{k+1} = term_k * (2n-1-k) * (n-1-k) * 2 / ((k+1)^2)          */
        idx_t nk1 = 2 * n - 1 - k;       /* 2n-1-k  (0 <= nk1 <= 2n-1) */
        idx_t n1k = n - 1 - k;           /* n-1-k   (0 <= n1k <= n-1) */
        uint64_t m = (uint64_t)nk1 * (uint64_t)n1k * 2u;    /* product fits in 64 bits */
        uint64_t denom = (k + 1);
        denom = denom * denom;          /* (k+1)^2 also fits in 64 bits */

#ifdef __cplusplus
        /* In a C++ build, use a 128‑bit intermediate only when the type
           seq_t is an integral type; otherwise rely on the class's operators. */
        if constexpr (std::is_integral_v<seq_t>) {
            unsigned __int128 num = (unsigned __int128)term * m;
            term = (seq_t)(num / denom);
        } else {
            seq_t prod = term * (seq_t)m;
            term = prod / denom;
        }
#else
        /* C build: seq_t is an unsigned integer type.  Compute in 128 bits. */
        unsigned __int128 num = (unsigned __int128)term * m;
        term = (seq_t)(num / denom);
#endif
        sum += term;
    }
    return sum;
}

/*
 * A194055  Natural fractal sequence of A000071 (Fibonacci numbers minus 1).
 * https://oeis.org/A194055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=1, a(3)=2; for n>3, let m=n-3. For k≥2, let
 *            L_k=fib(k+2). Find the smallest k such that m ≤ Σ_{i=2}^k L_i.
 *            Then a(n)=m-Σ_{i=2}^{k-1}L_i
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Handle the first block: 1,1,2 */
    if (n <= 3) {
        if (n == 1 || n == 2)
            return (seq_t)1;
        return (seq_t)2;
    }

    /* Skip the first block and work with Fibonacci block lengths */
    idx_t rem = n - 3;               /* remaining position within the blocks */
    for (idx_t k = 2;; ++k) {        /* k=2 corresponds to the second block */
        idx_t len = (idx_t)fib(k + 2); /* block length = F(k+2) */
        if (rem <= len)
            return (seq_t)rem;
        rem -= len;
    }
}

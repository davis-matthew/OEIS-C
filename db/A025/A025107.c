/*
 * A025107  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2
 *          ], s = (F(2), F(3), F(4), ...), t = A000201 (lower Wythoff
 *          sequence).
 * https://oeis.org/A025107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌈n/2⌉} F(i+1) * floor((n-i+2)⋅φ) where φ =
 *            (1+√5)/2, F is the Fibonacci sequence with F(0)=0, F(1)=1. s =
 *            (F(2), F(3), F(4), …) ; t = A000201 (lower Wythoff)
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset starts at 1 */

    /* φ constant */
    const double PHI = (1.0 + sqrt(5.0)) / 2.0;

    idx_t k = (n + 1) / 2;          /* ceil(n/2) */
    seq_t sum = 0;

    /* Fibonacci starting with F(2)=1, F(3)=2 */
    seq_t fib_prev = 1;   /* F(2) */
    seq_t fib_curr = 2;   /* F(3) */

    for (idx_t i = 1; i <= k; ++i)
    {
        idx_t j = n - i + 2;        /* argument for t */

        /* t(j) = floor(j * φ)  and j ≥ 2 for all n ≥ 1 */
        uint64_t tj_uint = (uint64_t)((double)j * PHI);   /* truncates to floor */
        seq_t tj = (seq_t)tj_uint;

        sum += fib_prev * tj;

        /* advance Fibonacci */
        seq_t next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = next;
    }

    return sum;
}

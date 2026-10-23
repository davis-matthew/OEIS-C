/*
 * A024878  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2
 *          ], s = (natural numbers >= 3), t = (F(2), F(3), F(4), ...).
 * https://oeis.org/A024878
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1}^{⌊n/2⌋} (i+2) * F(n-i+2) where F(0)=0, F(1)=1
 *            (i.e. t(j)=F(j+1)). Offset is 2, a(2) is first term
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;               /* undefined below the offset */

    /* ---------- compute Fibonacci numbers up to F(n+1) ---------------- */
    idx_t fib_len = (idx_t)n + 2;        /* need indices 0 … n+1 inclusive */
    seq_t *fib = (seq_t *)malloc((size_t)fib_len * sizeof(seq_t));
    if (!fib)
        return (seq_t)0;               /* allocation failure (unlikely) */

    fib[0] = (seq_t)0;
    fib[1] = (seq_t)1;
    for (idx_t i = 2; i <= n + 1; ++i)
        fib[i] = fib[i - 1] + fib[i - 2];

    /* ---------- compute the sum ------------------------------------- */
    idx_t k = n >> 1;                   /* floor(n/2) */
    seq_t result = (seq_t)0;

    for (idx_t i = 1; i <= k; ++i)
    {
        idx_t t_idx = n - i + 1;        /* we need F(t_idx+1) = F(n-i+2) */
        seq_t t_val = fib[t_idx + 1];
        seq_t s_val = (seq_t)(i + 2);   /* s(i) = i + 2 (natural numbers ≥ 3) */
        result += s_val * t_val;
    }

    free(fib);
    return result;
}

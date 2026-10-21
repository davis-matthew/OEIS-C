/*
 * A207873  Numerator of Z(n,1/2), where Z(n,x) is the n-th Zeckendorf
 *          polynomial.
 * https://oeis.org/A207873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Zeckendorf numerator: compute by greedy decomposition using Fibonacci numbers
   1,2,3,5,8,…; for each chosen term of index pos, add 1 << (max_pos‑pos)
   to the numerator. */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t fib[200];
    size_t fib_len = 0;
    fib[fib_len++] = (seq_t)1;
    fib[fib_len++] = (seq_t)2;
    while (1)
    {
        seq_t next = fib[fib_len - 2] + fib[fib_len - 1];
        if (next > (seq_t)n || next < fib[fib_len - 1])
            break;
        fib[fib_len++] = next;
        if (fib_len >= 200)
            break;
    }

    idx_t pos[200];
    size_t used = 0;
    idx_t max_pos = 0;
    seq_t remaining = (seq_t)n;
    for (size_t i = fib_len; i > 0; --i)
    {
        seq_t f = fib[i - 1];
        if (f <= remaining)
        {
            remaining -= f;
            pos[used++] = (idx_t)i;
            if ((idx_t)i > max_pos)
                max_pos = (idx_t)i;
        }
        if (remaining == 0)
            break;
    }

    seq_t numerator = 0;
    for (size_t j = 0; j < used; ++j)
    {
        idx_t shift = max_pos - pos[j];
        seq_t term = ((seq_t)1) << (size_t)shift;
        numerator += term;
    }
    return numerator;
}

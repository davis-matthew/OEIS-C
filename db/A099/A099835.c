/*
 * A099835  Bisection of A005117.
 * https://oeis.org/A099835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A005117(2*n) – the 2*n‑th squarefree number
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_squarefree(idx_t x)
{
    seq_t xx = (seq_t)x;
    for (idx_t p = 2; (seq_t)p * (seq_t)p <= xx; ++p) {
        seq_t psq = (seq_t)p * (seq_t)p;
        if (xx % psq == 0)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)  /* safety for offset <1; not expected to be called */
        return (seq_t)0;

    /* 2*n-th element of the squarefree numbers */
    idx_t target   = n * 2;
    idx_t count    = 0;
    idx_t candidate = 1;

    for (;; ++candidate) {
        if (is_squarefree(candidate)) {
            ++count;
            if (count == target)
                return (seq_t)candidate;
        }
    }
}

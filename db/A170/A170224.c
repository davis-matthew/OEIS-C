/*
 * A170224  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n ≥ 0 a(n) = 21·∑_{k=1}^{39} a(n‑k) – 231·a(n‑40) +
 *            c(n), where c(0)=1, c(1…39)=2, c(40)=1, c(n>40)=0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence follows from the rational generating function
   given in the OEIS entry.  The implementation keeps a ring
   buffer of the last 41 terms, so that only O(1) state is
   required and each call runs in O(n) time.  */
#include <stddef.h>

seq_t a(idx_t n)
{
    const size_t BUF_SZ = 41;            /* 40 previous terms + a(n) */
    seq_t buf[BUF_SZ];
    for (size_t j = 0; j < BUF_SZ; ++j)
        buf[j] = 0;

    seq_t sumPrev = 0;                   /* sum of the last up to 39 terms */

    for (idx_t i = 0; i <= n; ++i)
    {
        size_t idx = (size_t)(i % BUF_SZ);

        seq_t rhs;
        if (i == 0)
            rhs = 1;
        else if (i <= 39)
            rhs = 2;
        else if (i == 40)
            rhs = 1;
        else
            rhs = 0;

        seq_t term = sumPrev * (seq_t)21 + rhs;
        if (i >= 40)
        {
            size_t sub = (size_t)((i - 40) % BUF_SZ);
            term -= (seq_t)231 * buf[sub];
        }

        buf[idx] = term;

        sumPrev += term;
        if (i >= 39)
        {
            size_t rem = (size_t)((i - 39) % BUF_SZ);
            sumPrev -= buf[rem];
        }
    }

    return buf[(size_t)(n % BUF_SZ)];
}

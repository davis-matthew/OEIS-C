/*
 * A170028  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n ≥ 1: a(n) = 17 * (a(n‑1) + … + a(n‑35)) + rhs
 *            minus 153 * a(n‑36) if n ≥ 36, where rhs = 2 for 1 ≤ n ≤ 35, rhs
 *            = 1 for n = 36, rhs = 0 for n > 36. This recurrence comes from
 *            the generating function of A170028
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Return the n-th term of OEIS sequence A170028.  */
seq_t a(idx_t n)
{
    /* a(0) = 1 by definition.  */
    if (n == 0)
        return 1;

    /* Allocate a circular buffer to keep the last 36 terms.
       36 is the order of the recurrence.  */
    static const size_t BUF_SIZE = 36;
    seq_t* buf = (seq_t*)malloc(BUF_SIZE * sizeof(seq_t));
    if (!buf)
        return 0;   /* Out of memory – unlikely in the test harness. */

    /* Initialise the buffer with the known value a(0).  */
    for (size_t i = 0; i < BUF_SIZE; ++i)
        buf[i] = 0;
    buf[0] = 1;

    /* sum holds a(n‑1) + … + a(n‑35) for the current n.
       Initially for n = 1 this is just a(0).  */
    seq_t sum = 1;  /* a(0) */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* Determine the right-hand side coefficient.  */
        seq_t rhs;
        if (i <= 35)
            rhs = 2;
        else if (i == 36)
            rhs = 1;
        else
            rhs = 0;

        /* Compute the recurrence value.  */
        seq_t val = sum * 17;

        if (i >= 36)
        {
            /* Subtract 153 * a(i‑36).  a(i‑36) is stored at buffer
               index (i‑36) % BUF_SIZE.  */
            size_t idx_old = (size_t)(i - 36) % BUF_SIZE;
            val -= buf[idx_old] * 153;
        }

        val += rhs;

        /* Store the new value in the circular buffer.  */
        size_t idx_cur = (size_t)i % BUF_SIZE;
        buf[idx_cur] = val;

        /* Update the running sum for the next iteration:
           1. Add the new term.
           2. If the window has reached size 35, remove the oldest term a(i‑35).  */
        sum += val;
        if (i >= 35)
        {
            size_t idx_out = (size_t)(i - 35) % BUF_SIZE;
            sum -= buf[idx_out];
        }
    }

    /* The answer is stored at buf[n % BUF_SIZE].  */
    seq_t result = buf[(size_t)n % BUF_SIZE];
    free(buf);
    return result;
}

/*
 * A081675  Generalized Poly-Bernoulli numbers.
 * https://oeis.org/A081675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4*5^n - 3*4^n - 2^n)/6
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 985 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Recurrence: a(0)=0, a(1)=1, a(2)=8,
   a(n) = 11*a(n-1) - 38*a(n-2) + 40*a(n-3)  (n > 2) */
seq_t a(idx_t n)
{
    static seq_t *buf = NULL;
    static idx_t buf_size   = 0;        /* allocated array size          */
    static idx_t max_computed = 0;      /* highest index already computed */

    /* Initial allocation and seed values */
    if (buf == NULL) {
        buf_size = (n > 2 ? n + 1 : 3);
        buf = mem_new(seq_t, buf_size);
        buf[0] = (seq_t)0;
        if (buf_size > 1) buf[1] = (seq_t)1;
        if (buf_size > 2) buf[2] = (seq_t)8;
        max_computed = (buf_size > 2) ? 2 : (buf_size - 1);
    }
    /* Grow the buffer if needed */
    else if (n + 1 > buf_size) {
        idx_t new_size = n + 1;
        buf = mem_resize(seq_t, buf, new_size);
        buf_size = new_size;
    }

    /* Return a value that has already been computed */
    if (n <= max_computed)
        return buf[n];

    /* Compute missing values up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* i > 2 for all iterations here */
        buf[i] = 11 * buf[i - 1] - 38 * buf[i - 2] + 40 * buf[i - 3];
    }

    max_computed = n;
    return buf[n];
}

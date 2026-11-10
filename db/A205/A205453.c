/*
 * A205453  s(A205451), where s(j)=Fibonacci(2j).
 * https://oeis.org/A205453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(2*j), where j = A205451(n) is found by scanning
 *            Fibonacci numbers mod n for the first repeated residue among
 *            even-indexed terms
 * Range:     u8 n <= 24, u16 n <= 52, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t A205453_j(idx_t n)
{
    idx_t *first = (idx_t *)calloc((size_t)n, sizeof(idx_t));
    if (!first)
        return 0;

    idx_t f_prev = 0;
    idx_t f_curr = 1 % n;
    idx_t k = 1;

    for (;;)
    {
        idx_t f_next = f_prev + f_curr;
        if (f_next >= n)
            f_next -= n;
        f_prev = f_curr;
        f_curr = f_next;
        ++k;

        if ((k & 1) == 0) {
            idx_t i = k >> 1;
            if (first[f_curr] != 0) {
                idx_t r = first[f_curr];
                free(first);
                return r;
            }
            first[f_curr] = i;
        }

        if (k > (idx_t)2 * n + 2) {
            free(first);
            return 0;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t j = A205453_j(n);
    idx_t steps = 2 * j;

    seq_t x = 0, y = 1;
    for (idx_t i = 0; i < steps; ++i) {
        seq_t t = x + y;
        x = y;
        y = t;
    }
    return x;
}

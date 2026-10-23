/*
 * A130261  Partial sums of the 'lower' even Fibonacci Inverse A130259.
 * https://oeis.org/A130261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A130261: Partial sums of the 'lower' even Fibonacci Inverse A130259.
    Formula: a(n) = (n+1)*A130259(n) - A001519(A130259(n)+1) + 1
    where A001519(k) = Fib(2*k-1).
    The partial sums a(n) are defined by   a(0)=0,
    a(n) = a(n‑1) + A130259(n‑1)  for n≥1.
    This function computes a(n) for any n≥0 using the recurrence
    that follows from the formula.  A(n) is found as the largest
    value satisfying  a(n‑1) = (n)*A(n‑1) - Fib(2*A(n‑1)+1) + 1. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t A = 1;            /* current value of A(n) (starts with A(0)=1) */
    seq_t sum = 0;          /* sum of A's so far, i.e. a(current indeks) */

    for (idx_t i = 0; i < n; ++i) {
        /* Adjust A upward while the recurrence still holds for the current sum. */
        while (1) {
            seq_t cand = A + 1;
            idx_t fib_idx = (idx_t)(cand * 2 + 1);
            seq_t fib_val = fib(fib_idx);

            /* lhs = (i+1)*cand - fib_val + 1   must equal sum */
            seq_t lhs = (seq_t)(i + 1) * cand - fib_val + 1;
            if (lhs == sum) {
                A = cand;
                continue;
            }
            break;
        }
        /* Now A satisfies the recurrence for the current sum. */
        sum += A;           /* a(i+1) = sum of A's up to index i */
    }

    return sum;
}

/*
 * A014241  a(n) = ((n+1)-st Fibonacci number) - (n-th non-Fibonacci number).
 * https://oeis.org/A014241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A000045(n+1) - A001690(n)
 * Range:     i8 n <= 11, i16 n <= 22, i32 n <= 45, i64 n <= 91, i128 n <= 183,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Outside the OEIS offset; behaviour beyond the offset is irrelevant */
        return 0;
    }

    /* Fibonacci value at position n+1 */
    seq_t fib_val = fib(n + 1);

    /* Find the n‑th non‑Fibonacci number */
    idx_t remaining = n;

    /* F_k and F_{k+1} (starting with 1, 1) */
    seq_t Fk   = 1;   /* F_1 */
    seq_t Fk1  = 1;   /* F_2 */

    for (;;) {
        /* Difference between consecutive Fibonacci numbers */
        seq_t diff = Fk1 - Fk;

        /* Numbers strictly between F_k and F_{k+1}:
           gap = max(0, diff - 1) */
        seq_t gap = diff - 1;
        if (diff <= 1) {
            gap = 0;
        }

        /* If the desired non‑Fibonacci lies in this gap */
        if ((seq_t)remaining <= gap) {
            seq_t non_fib = Fk + (seq_t)remaining;
            return fib_val - non_fib;
        }

        /* Skip this gap and continue */
        remaining -= (idx_t)gap;   /* gap fits in idx_t because remaining ≤ 10 000 */

        /* Advance Fibonacci sequence */
        seq_t next = Fk + Fk1;
        Fk  = Fk1;
        Fk1 = next;
    }
}

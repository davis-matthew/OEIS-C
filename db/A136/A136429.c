/*
 * A136429  a(n) = Sum_{k=0..n} F(k+1)^2*F(n-k+1)^2 where F = Fibonacci
 *          numbers (A000045).
 * https://oeis.org/A136429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A136429
 * Formula: a(n) = Σ_{k=0..n} F(k+1)^2 * F(n-k+1)^2, where F is the Fibonacci sequence.
 * Recurrence: a(n+6) = 4*a(n+5) - 10*a(n+3) + 4*a(n+1) - a(n)
 * Initial terms: a(0)=1, a(1)=2, a(2)=9, a(3)=26, a(4)=84, a(5)=250
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values for n = 0..5. */
    static const seq_t base[6] = {1, 2, 9, 26, 84, 250};

    if (n < 6)
        return base[n];

    /* Cached sequence values. */
    static seq_t *seq = NULL;
    static idx_t len = 0;   /* number of elements currently stored */

    /* Initialise cache on first call. */
    if (!seq) {
        seq = (seq_t *)malloc(6 * sizeof(seq_t));
        for (idx_t i = 0; i < 6; ++i)
            seq[i] = base[i];
        len = 6;
    }

    /* Compute missing values up to n. */
    if (n >= len) {
        seq = (seq_t *)realloc(seq, (size_t)(n + 1) * sizeof(seq_t));
        for (idx_t i = len; i <= n; ++i) {
            /* a(i) = 4*a(i-1) - 10*a(i-3) + 4*a(i-5) - a(i-6)  */
            seq[i] = 4 * seq[i - 1] - 10 * seq[i - 3] + 4 * seq[i - 5] - seq[i - 6];
        }
        len = n + 1;
    }

    return seq[n];
}

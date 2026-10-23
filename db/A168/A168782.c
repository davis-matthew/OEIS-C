/*
 * A168782  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^19 = I.
 * https://oeis.org/A168782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1;
 * for n≥1, a(n)=7*(a(n-1)+...+a(n-18)) – 28*a(n-19) + r(n),
 * where r(n)=1 if n=0, 2 if 1≤n≤18, 1 if n=19, and 0 otherwise. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Buffer holding the previous 19 terms, initially zero. */
    seq_t prev[19] = {0};

    seq_t a_i = 0;
    for (idx_t i = 0; i <= n; ++i) {
        /* The forcing term r(i). */
        seq_t r;
        if (i == 0)          r = 1;
        else if (i <= 18)    r = 2;
        else if (i == 19)    r = 1;
        else                 r = 0;

        /* Compute the sum of 7*a(i-k) for k=1..18. */
        seq_t sum = 0;
        for (idx_t j = 0; j < 18; ++j)
            sum += 7 * prev[j];

        /* Subtract 28*a(i-19) if i≥19. */
        if (i >= 19)
            sum -= 28 * prev[18];

        sum += r;
        a_i = sum;

        /* Shift the ring buffer: newest term becomes prev[0]. */
        for (idx_t j = 18; j > 0; --j)
            prev[j] = prev[j - 1];
        prev[0] = a_i;
    }
    return a_i;
}

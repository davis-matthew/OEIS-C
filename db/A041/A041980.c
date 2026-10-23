/*
 * A041980  Numerators of continued fraction convergents to sqrt(513).
 * https://oeis.org/A041980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numerators of continued fraction convergents to sqrt(513)
   Recurrence: a(n) = 27542702*a(n-16) - a(n-32),   for n>=32
   Initial values are generated from the continued‐fraction partial quotients
   of sqrt(513). */

/* Include standard headers needed for size_t and dynamic allocation */
#include <stdlib.h>

/* The function to compute the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction of sqrt(513) after the initial 22 */
    static const seq_t period[16] = {
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)5,
        (seq_t)1, (seq_t)4, (seq_t)5, (seq_t)2,
        (seq_t)5, (seq_t)4, (seq_t)1, (seq_t)5,
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)44
    };

    /* Allocate enough space to store values from 0 up to n */
    size_t need = (size_t)n + 1;
    seq_t *vals = (seq_t*)malloc(need * sizeof(seq_t));
    if (!vals) {
        /* In case of allocation failure, return 0 (unlikely for small n) */
        return (seq_t)0;
    }

    /* Compute terms 0 to 31 using the convergent recurrence */
    seq_t p_prev2 = 0; /* p_{-2} */
    seq_t p_prev1 = 1; /* p_{-1} */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t a_k;
        if (k == 0) {
            a_k = (seq_t)22;
        } else {
            a_k = period[(k - 1) % 16];
        }
        seq_t p_k = a_k * p_prev1 + p_prev2;
        vals[k] = p_k;
        /* Update for next step */
        p_prev2 = p_prev1;
        p_prev1 = p_k;
    }

    /* For n >= 32, replace terms computed beyond 31 with recurrence */
    if (n >= 32) {
        for (idx_t k = 32; k <= n; ++k) {
            vals[k] = (seq_t)27542702 * vals[k - 16] - vals[k - 32];
        }
    }

    seq_t result = vals[n];
    free(vals);
    return result;
}

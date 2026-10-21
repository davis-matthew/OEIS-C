/*
 * A068102  a(n) = n! * 2^n * Sum_{i=1..n} 1/(i*2^i).
 * https://oeis.org/A068102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n!*2^n*∑_{i=1..n} 1/(i*2^i) Recurrence: a(0)=0,
 *            a(n+1)=2*(n+1)*a(n)+n! (Offset: 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h> /* for size_t */

/* Memoised storage for a(n) and factorial values.
 * The arrays grow as needed; index 0 holds the values for n=0
 * (used internally for the recurrence). */
static seq_t *a_vals   = NULL;
static seq_t *fact_vals = NULL;
static size_t cached   = 0;   /* number of elements currently stored (>=1) */

/* Return a(n) for n >= 1. Behaviour for n < 1 is undefined. */
seq_t a(idx_t n)
{
    /* Ensure the arrays contain indices 0..n. */
    size_t needed = (size_t)n + 1;          /* include index 0 */
    if (cached < needed) {
        /* Grow arrays (simple linear growth). */
        size_t new_size = needed * 2;
        a_vals   = (seq_t *)realloc(a_vals,   new_size * sizeof(seq_t));
        fact_vals = (seq_t *)realloc(fact_vals, new_size * sizeof(seq_t));

        /* Initialise from the first missing index up to n. */
        for (size_t i = cached; i < needed; ++i) {
            if (i == 0) {
                a_vals[0]    = 0;
                fact_vals[0] = 1;
            } else {
                /* fact_vals[i] = fact_vals[i-1] * i */
                fact_vals[i] = fact_vals[i-1] * (seq_t)i;

                /* a_vals[i] = 2*i*a_vals[i-1] + fact_vals[i-1] */
                seq_t term = ((seq_t)2 * (seq_t)i) * a_vals[i-1];
                a_vals[i] = term + fact_vals[i-1];
            }
        }
        cached = needed;
    }
    return a_vals[n];
}
